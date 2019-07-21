#include <source/nn/NeuralNetFactory.h>
#include <source/ga/ChromosomeFactory.h>
#include "iAnt_controller.h"

static CRange<Real> NN_OUTPUT_RANGE(-1.0f, 1.0f);
static CRange<Real> WHEEL_ACTUATION_RANGE(-16.0f, 16.0f);

/*****
 * Initialize most basic variables and objects here. Most of the setup should be done in the Init(...) function instead
 * of here where possible.
 *****/
iAnt_controller::iAnt_controller() :
    compass(NULL),
    motorActuator(NULL),
    proximitySensor(NULL),
    lightSensor(NULL),
    distanceTolerance(0.0),
    searchStepSize(0.0),
    robotForwardSpeed(0.0),
    robotRotationSpeed(0.0),
    RNG(NULL),
    loopFunctions(NULL),
    isHoldingFood(false),
    networkInitalized(false)
{}

/*****
 * Initialize the controller via the XML configuration file. ARGoS typically wants objects & variables initialized here
 * instead of in the constructor(s).
 *****/
void iAnt_controller::Init(TConfigurationNode& node) {

    /* Shorter names, please. #This_Is_Not_Java */
    typedef CCI_PositioningSensor            CCI_PS;
    typedef CCI_DifferentialSteeringActuator CCI_DSA;
    typedef CCI_FootBotProximitySensor       CCI_FBPS;

    /* Initialize the robot's actuator and sensor objects. */
    motorActuator   = GetActuator<CCI_DSA>("differential_steering");
    compass         = GetSensor<CCI_PS>   ("positioning");
    proximitySensor = GetSensor<CCI_FBPS> ("footbot_proximity");
    lightSensor     = GetSensor  <CCI_FootBotLightSensor>("footbot_light");

    CVector2 p(GetPosition());
    startPosition = CVector3(p.GetX(), p.GetY(), 0.0);
}

/*****
 * Primary control loop for this controller object. This function will execute the CPFA logic using the CPFA 
 * enumeration flag once per frame.
 *****/
void iAnt_controller::ControlStep() {

    if(!networkInitalized){
        NeuralNetFactory factory;
        network = factory.build(loopFunctions->chromosome, ChromosomeFactory::INPUT_COUNT, ChromosomeFactory::OUTPUT_COUNT);
        networkInitalized = true;
    }

    //update inputs
    network->getInputs().at(0)->setValue(compass->GetReading().Orientation.GetW());
    network->getInputs().at(1)->setValue(compass->GetReading().Orientation.GetX());
    network->getInputs().at(2)->setValue(compass->GetReading().Orientation.GetY());
    network->getInputs().at(3)->setValue(compass->GetReading().Orientation.GetZ());

    network->getInputs().at(4)->setValue(isHoldingFood? 1 : 0);
    network->getInputs().at(5)->setValue(IsNearFood()? 1 : 0);

    int frontIndexes[6] = {21, 22, 23, 0, 1, 2};
    Real front = maxProximity(frontIndexes);
    int leftIndexes[6] = {3, 4, 5, 6, 7, 8};
    Real left = maxProximity(leftIndexes);
    int backIndexes[6] = {9, 10, 11, 12, 13, 14};
    Real back = maxProximity(backIndexes);
    int rightIndexes[6] = {15, 16, 17, 18, 19, 20};
    Real right = maxProximity(rightIndexes);

    network->getInputs().at(6)->setValue(front);
    network->getInputs().at(7)->setValue(left);
    network->getInputs().at(8)->setValue(back);
    network->getInputs().at(9)->setValue(right);

    network->getInputs().at(10)->setValue(IsNearPheromone() ? 1 : 0);

    const CCI_FootBotLightSensor::TReadings& tReadings = lightSensor->GetReadings();
    int numIndices = 6;

    int frontLight = maxLightIndex(tReadings, frontIndexes,numIndices);
    int leftLight = maxLightIndex(tReadings, leftIndexes, numIndices);
    int backLight = maxLightIndex(tReadings, backIndexes, numIndices);
    int rightLight = maxLightIndex(tReadings, rightIndexes, numIndices);

    network->getInputs().at(11)->setValue(tReadings[frontLight].Value);
    network->getInputs().at(12)->setValue(tReadings[leftLight].Value);
    network->getInputs().at(13)->setValue(tReadings[backLight].Value);
    network->getInputs().at(14)->setValue(tReadings[rightLight].Value);

    network->update();

    NN_OUTPUT_RANGE.MapValueIntoRange(
            m_fLeftSpeed,               // value to write
            network->getOutputs().at(0)->getCachedValue(), // value to read
            WHEEL_ACTUATION_RANGE       // target range (here [-16:16])
    );
    NN_OUTPUT_RANGE.MapValueIntoRange(
            m_fRightSpeed,              // value to write
            network->getOutputs().at(1)->getCachedValue(), // value to read
            WHEEL_ACTUATION_RANGE       // target range (here [-16:16])
    );
    motorActuator->SetLinearVelocity(
            m_fLeftSpeed,
            m_fRightSpeed);

    if(network->getOutputs().at(2)->getCachedValue() > 0){
        layPheromone();
    }

    SetHoldingFood();
}

Real iAnt_controller::maxProximity(int *sensorIndex) {
    Real maxProximity = 0;

    for(int i = 0; i < 6; i++) {
        Real proximityValue = proximitySensor->GetReadings().at(sensorIndex[i]).Value;
        if (proximityValue > maxProximity) {
            maxProximity = proximityValue;
        }
    }

    return maxProximity;
}

int iAnt_controller::maxLightIndex(CCI_FootBotLightSensor::TReadings tReadings,
                                   int sensorIndex[], int numIndices) {
    Real maxVal = 0.0;

    int maxIndex = sensorIndex[0];

    for(int i = 0; i < numIndices; i++) {
        if (maxVal > tReadings[sensorIndex[i]].Value){
            maxVal= tReadings[sensorIndex[i]].Value;
            maxIndex = i;
        };
    }
    return maxIndex;
}

/*****
 * After pressing the reset button in the GUI, this controller will be set to default factory settings like at the
 * start of a simulation.
 *****/
void iAnt_controller::Reset() {
    NeuralNetFactory factory;
    network = factory.build(loopFunctions->chromosome, ChromosomeFactory::INPUT_COUNT, ChromosomeFactory::OUTPUT_COUNT);

    /* Reset all local variables. */
    isHoldingFood       = false;
}

/*****
 * Check if the iAnt is finding food. This is defined as the iAnt being within
 * the distance tolerance of the position of a food item. If the iAnt has found
 * food then the appropriate boolean flags are triggered.
 *****/
void iAnt_controller::SetHoldingFood() {

    /* Is the iAnt already holding food? */
    if(!IsHoldingFood()) {

        /* No, the iAnt isn't holding food. Check if we have found food at our
           current position and update the food list if we have. */

        vector<CVector2> newFoodList;
        vector<CColor>   newFoodColoringList;
        size_t i = 0, j = 0;

        for(i = 0; i < loopFunctions->FoodList.size(); i++) {
            if((GetPosition() - loopFunctions->FoodList[i]).SquareLength() < loopFunctions->FoodRadiusSquared) {
                /* We found food! Calculate the nearby food density. */
                isHoldingFood = true;
                //SetLocalResourceDensity();
                j = i + 1;
                break;
            } else {
                /* Return this unfound-food position to the list */
                newFoodList.push_back(loopFunctions->FoodList[i]);
                newFoodColoringList.push_back(CColor::BLACK);
            }
        }

        for( ; j < loopFunctions->FoodList.size(); j++) {
            newFoodList.push_back(loopFunctions->FoodList[j]);
            newFoodColoringList.push_back(CColor::BLACK);
        }

        /* We picked up food. Update the food list minus what we picked up. */
        if(IsHoldingFood()) {
            loopFunctions->FoodList = newFoodList;
        }
    }
    /* Drop off food: We are holding food and have reached the nest. */
    else if((GetPosition() - loopFunctions->NestPosition).SquareLength() < loopFunctions->NestRadiusSquared) {
        isHoldingFood = false;
        loopFunctions->foodReturned++;
    }
}

bool iAnt_controller::IsNearFood() {
    for(int i = 0; i < loopFunctions->FoodList.size(); i++) {
        if ((GetPosition() - loopFunctions->FoodList[i]).SquareLength() < loopFunctions->FoodRadiusSquared) {
            return true;
        }
    }
    return false;
}

bool iAnt_controller::IsNearPheromone() {
    for(int i = 0; i < loopFunctions->Pheromones.size(); i++) {
        if (loopFunctions->Pheromones[i].IsActive() && ((GetPosition() - loopFunctions->Pheromones[i].GetLocation()).SquareLength() < loopFunctions->FoodRadiusSquared)) {
            return true;
        }
    }
    return false;
}

void iAnt_controller::layPheromone() {
    Real timeInSeconds = (Real)(loopFunctions->SimTime / loopFunctions->TicksPerSecond);
    if (!IsNearPheromone()) {
        iAnt_pheromone sharedPheromone(GetPosition(), timeInSeconds, loopFunctions->RateOfPheromoneDecay);
        loopFunctions->Pheromones.push_back(sharedPheromone);
    }
    else{
        for(int i = 0; i < loopFunctions->Pheromones.size(); i++) {
            if (loopFunctions->Pheromones[i].IsActive() && ((GetPosition() - loopFunctions->Pheromones[i].GetLocation()).SquareLength() < loopFunctions->FoodRadiusSquared)) {
                loopFunctions->Pheromones[i].Reset(timeInSeconds);
            }
        }
    }
}



/*****
 * Return the robot's 2D position on the arena.
 *****/
CVector2 iAnt_controller::GetPosition() {
    /* The robot's compass sensor gives us a 3D position. */
    CVector3 position3D = compass->GetReading().Position;
    /* Return the 2D position components of the compass sensor reading. */
    return CVector2(position3D.GetX(), position3D.GetY());
}

REGISTER_CONTROLLER(iAnt_controller, "iAnt_controller")
