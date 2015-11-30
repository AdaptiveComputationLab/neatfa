#ifndef IANT_CONTROLLER_H_
#define IANT_CONTROLLER_H_

#include <argos3/core/control_interface/ci_controller.h>
#include <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>
#include <argos3/plugins/robots/generic/control_interface/ci_positioning_sensor.h>
#include <argos3/plugins/robots/generic/control_interface/ci_differential_steering_actuator.h>
#include <argos3/plugins/robots/generic/control_interface/ci_leds_actuator.h>
#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_proximity_sensor.h>
#include <argos3/plugins/robots/generic/control_interface/ci_leds_actuator.h>
#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_light_sensor.h>
#include <argos3/core/utility/math/rng.h>
#include <source/iAnt_loop_functions.h>
#include <source/nn/Perceptron.h>
#include <source/nn/NeuralNetwork.h>
#include <source/ga/Chromosome.h>

using namespace argos;
using namespace std;

class iAnt_loop_functions;

/*****
 * The brain of each iAnt robot which implements the Central Place Foraging Algorithm (CPFA).
 *****/
class iAnt_controller : public CCI_Controller {

public:

    /* constructor and destructor */
    iAnt_controller();
    virtual ~iAnt_controller() {}

    /* CCI_Controller Inherited Functions */
    void Init(TConfigurationNode& node);
    void ControlStep();
    void Reset();

    /* public helper functions */
    bool IsHoldingFood() { return isHoldingFood; }
    void SetLoopFunctions(iAnt_loop_functions* lf) { loopFunctions = lf; }
    CVector2 GetPosition();
    CVector3 GetStartPosition() { return startPosition; }

private:

    Chromosome* chromosome;
    NeuralNetwork* network;
    bool networkInitalized;

    Real m_fLeftSpeed;
    Real m_fRightSpeed;

    /* foot-bot components: sensors and actuators */
    CCI_PositioningSensor*            compass;
    CCI_DifferentialSteeringActuator* motorActuator;
    CCI_FootBotProximitySensor*       proximitySensor;
    CCI_FootBotLightSensor*           lightSensor;

    /* iAnt controller parameters */
    Real             distanceTolerance;
    Real             searchStepSize;
    Real             robotForwardSpeed;
    Real             robotRotationSpeed;
    CRange<CRadians> angleToleranceInRadians;

    /* robot internal variables & statistics */
    CRandom::CRNG*       RNG;
    iAnt_loop_functions* loopFunctions;
    CVector3             startPosition;

    bool   isHoldingFood;

private:

    void SetHoldingFood();
    bool IsNearFood();
    bool IsNearPheromone();
    Real maxProximity(int *sensorIndex);
    int maxLightIndex(CCI_FootBotLightSensor::TReadings tReadings, int sensorIndex[],
                      int numIndicies);

    void layPheromone();
};

#endif /* IANT_CONTROLLER_H_ */
