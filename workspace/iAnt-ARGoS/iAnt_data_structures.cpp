#include "iAnt_data_structures.h"

void iAnt_data_structures::Init(TConfigurationNode& node) {
	CPFA.Init(GetNode(node, "CPFA"));
	navigation.Init(GetNode(node, "navigation"));
	food.Init(GetNode(node, "food"));
}

iAnt_data_structures::CPFA::CPFA():
	state(SET_SEARCH_LOCATION),
	travelProbability(0.75),
	searchProbability(0.25),
	uninformedSearchCorrelation(45.0),
	informedSearchDecay(0.01),
	siteFidelityRate(0.15),
	pheromoneRate(0.15),
	pheromoneDecayRate(0.01),
	informed(false),
	searchTime(0)
{}

void iAnt_data_structures::CPFA::Init(TConfigurationNode& node) {
	/* We get input in degrees from the XML file for the user's ease of use. */
	CDegrees angleInDegrees;

	/* Set CPFA structure variables to values assigned from the XML file. */
    GetNodeAttribute(node, "travelProbability", travelProbability);
    GetNodeAttribute(node, "searchProbability", searchProbability);
    GetNodeAttribute(node, "uninformedSearchCorrelation", angleInDegrees);
    GetNodeAttribute(node, "informedSearchDecay", informedSearchDecay);
    GetNodeAttribute(node, "siteFidelityRate", siteFidelityRate);
    GetNodeAttribute(node, "pheromoneRate", pheromoneRate);
    GetNodeAttribute(node, "pheromoneDecayRate", pheromoneDecayRate);

    /* Convert the input from angleInDegrees to Radians. */
    uninformedSearchCorrelation = ToRadians(angleInDegrees);
}

/*iAnt_data_structures::*/pheromoneWaypoint::pheromoneWaypoint() :
		location(CVector2()),
		weight(0),
		lastUpdated(0)
{}

/*iAnt_data_structures::*/pheromoneWaypoint::pheromoneWaypoint(CVector2 loc, double w, int tick) :
		location(loc),
		weight(w),
		lastUpdated(tick)
{}


iAnt_data_structures::navigation::navigation() :
    distanceTolerance(0.0),
    angleTolerance(-CRadians::ZERO, CRadians::ZERO),
    arenaSize(0.0, 0.0),
    forageRangeX(-0.0, 0.0),
    forageRangeY(-0.0, 0.0),
    position(0.0, 0.0),
    target(0.0, 0.0),
    nestPosition(0.0, 0.0),
    nestRadiusSquared(0.0),
    searchStepSize(0.0),
    searchRadiusSquared(0.0),
    maxSpeed(0.0)
{}

void iAnt_data_structures::navigation::Init(TConfigurationNode& node) {
	/* We get input in degrees from the XML file for the user's ease of use. */
	CDegrees angleInDegrees;

	/* Get the user specified foraging range, [-rangeX, rangeX], [-rangeY, rangeY]
	 * rangeX cannot be larger than 0.5 * arenaSize.GetX() and rangeY cannot be
	 * larger than 0.5 * arenaSize.GetY() respectively. */
	CVector2 rangeX, rangeY;

	/* Set navigation structure variables to values assigned from the XML file. */
    GetNodeAttribute(node, "distanceTolerance", distanceTolerance);
    GetNodeAttribute(node, "angleTolerance",    angleInDegrees);
    GetNodeAttribute(node, "arenaSize",         arenaSize);
    GetNodeAttribute(node, "forageRangeX",      rangeX);
    GetNodeAttribute(node, "forageRangeY",      rangeY);
    GetNodeAttribute(node, "nestPosition",      nestPosition);
    GetNodeAttribute(node, "nestRadius",        nestRadiusSquared);
    GetNodeAttribute(node, "searchStepSize",    searchStepSize);
    GetNodeAttribute(node, "searchRadius",      searchRadiusSquared);
    GetNodeAttribute(node, "max_speed",         maxSpeed);

    /* The XML input is the radius, so multiply these by themselves to get the squared value. */
    nestRadiusSquared *= nestRadiusSquared;
    searchRadiusSquared *= searchRadiusSquared;

    /* Convert the input from angleInDegrees to radians. */
    angleTolerance.Set(-ToRadians(angleInDegrees), ToRadians(angleInDegrees));

    forageRangeX.Set(rangeX.GetX(), rangeX.GetY());
    forageRangeY.Set(rangeY.GetX(), rangeY.GetY());
}

iAnt_data_structures::food::food() :
	isHoldingFoodItem(false),
	hasActivePheromone(false),
	resourcesCollected(0),
	foodRadiusSquared(0.0)
{}

void iAnt_data_structures::food::Init(TConfigurationNode& node) {
	isHoldingFoodItem = false;
	hasActivePheromone = false;
	resourcesCollected = 0;

	GetNodeAttribute(node, "foodRadius", foodRadiusSquared);

	foodRadiusSquared *= foodRadiusSquared;
}