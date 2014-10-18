#ifndef IANT_CONTROLLER_H_
#define IANT_CONTROLLER_H_

#include "iAnt_pheromone.h"
#include <argos3/core/control_interface/ci_controller.h>
#include <argos3/plugins/robots/generic/control_interface/ci_differential_steering_actuator.h>
#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_proximity_sensor.h>
#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_motor_ground_sensor.h>
#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_light_sensor.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/plugins/robots/generic/control_interface/ci_positioning_sensor.h>

using namespace argos;
using namespace std;

class iAnt_controller : public CCI_Controller {

	public:

		iAnt_controller();
		~iAnt_controller();

		bool IsInTheNest();   // true or false: Am I in the nest?
		bool IsFindingFood(); // true or false: Am I finding food (i.e. currently over a food item location)?
		bool IsHoldingFood(); // true or false: AM I holding a food item?
		void PickupFood();    // the robot is picking up a food item
		void DropOffFood();   // the robot is dropping off a food item
		void UpdateFoodList(vector<CVector2> newFoodPositions); // update food position list
		void UpdatePosition(CVector2 newPosition); // update iAnt's position
		void UpdateTime(long int newTime);         // update simulation time
		CVector2 Position(); // return the robot's position
		void TargetPheromone(iAnt_pheromone p); // update target pheromone

		/* CCI_Controller inherited functions */
		void Init(TConfigurationNode& node); // initialize variables based on XML file
		void ControlStep(); // main logic control for the robot
		void Reset();       // when Argos3 GUI is reset, reset variables
		void Destroy();     // after experiment, clean up memory etc.

	private:

		CCI_DifferentialSteeringActuator *steeringActuator; // controls the robot's motor speeds
		CCI_FootBotProximitySensor       *proximitySensor;  // detects nearby objects to prevent collision
		CCI_FootBotMotorGroundSensor     *groundSensor;     // detects food items & nest (color changes)
		//CCI_FootBotLightSensor           *lightSensor;      // detects nest-light for navigation control
	    CCI_PositioningSensor            *m_pcPositioning;  // used for compass/heading

		CRandom::CRNG *RNG; // random number generator

		vector<CVector2> foodPositions; // list of food item positions, updated by loop_functions

		bool holdingFood; // Is the robot carrying food? yes = true, no = false
		bool informed; // is site fidelity or pheromones active

		float resourceDensity; // number of food items around the last found food position

		CVector2 position; // robot's position on the arena
		CVector2 target; // robot's current target or location to move to
		CVector2 fidelityPosition; // robot's remembered fidelity location
		CVector2 nestPosition; // the position of the center of the nest
		CVector2 arenaSize; // rectangular grid size "GetX()" by "GetY()"

		Real searchRadiusSquared; // radius of search for resourceDensity
		Real distanceTolerance; // distance to trigger collision detection
		Real travelGiveupProbability; // %-chance of traveling, from [0.0, 1.0]
		Real searchGiveupProbability; // %-chance of searching, from [0.0, 1.0]
		Real searchStepSize; // vector length for each search "step"
		Real maxSpeed; // maximum motor speed, configured in XML
		Real informedSearchDecay;
		Real siteFidelityRate; // % chance of using site fidelity
		Real pheromoneRate; // % chance of laying pheromones
		Real pheromoneDecayRate; // %-rate that pheromones decay [0.0, 10.0]

		long int simTime; // frame count for simulation
		long int searchTime; // time spent searching

		CRadians uninformedSearchCorrelation; // radian angle turned during searching [0.0, 4.0PI]

		CRange<CRadians> angleTolerance; // angle tolerance range to go straight

		iAnt_pheromone targetPheromone; // pheromone trail iAnt is following
		iAnt_pheromone sharedPheromone; // pheromone iAnt is sharing

		// state machine parameters, NOTE: the algorithm used is NOT a true state machine model
		enum CPFA {
			INACTIVE,
			DEPARTING,
			SEARCHING,
			RETURNING,
		} CPFA;

		// CPFA implementation functions
		void inactive();
		void departing();
		void searching();
		void returning();

		// CPFA helper functions
		void senseLocalResourceDensity();
		CVector2 setPositionInBounds(CVector2 rawPosition);

		// private helper functions for motion control and navigation
		void setRandomSearchLocation(); // set target to a random location
		bool collisionDetection(); // detect collisions and turn appropriately
		CRadians lawOfCosines(CVector2& A, CVector2& B, CVector2& C); // helper for getVectorToPosition()
		Real getSignOfRotationAngle(CVector2& A, CVector2& B, CVector2& C); // helper for lawOfCosines()
		double getHeading();
		CVector2 getVectorToLight(); // calculate heading towards the nest-light
		CVector2 getVectorToPosition(const CVector2& targetPosition); // calculate heading towards robot target
		void setWheelSpeed(); // set wheel speeds based on heading and target position
		void setWheelSpeed(const CVector2& heading); // set wheel speeds based on desired heading
};

#endif /* IANT_CONTROLLER_H_ */