#ifndef IANT_PHEROMONE_H_
#define IANT_PHEROMONE_H_

#include <argos3/core/utility/math/vector2.h>

using namespace argos;
using namespace std;

/*****
 * Implementation of the iAnt Pheromone object used by the iAnt CPFA. iAnts build and maintain a list of these pheromone waypoint objects to use during
 * the informed search component of the CPFA algorithm.
 *****/
class iAnt_pheromone {

    public:

        /* constructor function */
		iAnt_pheromone(CVector2 newLocation, Real newTime, Real newDecayRate);

        /* public helper functions */
        void             Update(Real time);
        void             Deactivate();
		CVector2         GetLocation();
		Real             GetWeight();
        bool             IsActive();
	    void             Reset(Real time);

	private:

        /* pheromone position variables */
		CVector2         location;

        /* pheromone component variables */
		Real lastUpdated;
		Real decayRate;
		Real weight;
		Real threshold;
};

#endif /* IANT_PHEROMONE_H_ */