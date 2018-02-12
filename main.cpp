#include "GameGOAPLib.h"
#include "demo/Action/AttackAction.h"
#include "demo/Action/GoToEnemyAction.h"
#include "demo/Action/SearchAction.h"
#include "demo/WorldStateInspector.h"
#include "Blackboard.h"

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

using namespace GameGOAP;
using namespace std;
using namespace std::chrono;

int main(void)
{
	std::cout << "Example running...\n";

	// create action list
	std::vector<Action*> actions;
	actions.reserve(MAX_ACTIONS);

	// create variable map
	std::vector<std::string> variableMap;

	// create initial and goal states
	WorldState* initialState = new WorldState;
	WorldState* goalState = new WorldState;

	// create factory
	ActionFactory* factory = new ActionFactory;
	factory->registerClass("AttackAction", AttackAction::create);
	factory->registerClass("GoToEnemyAction", GoToEnemyAction::create);
	factory->registerClass("SearchAction", SearchAction::create);
	
	// load from file
	GOAPLoader* loader = new GOAPLoader;
	loader->loaGOAP("GOAPTest1.xml", factory, actions, variableMap, *initialState, *goalState, true);

	// check actions
	for (auto action : actions)
	{
		std::cout << action->getName() << " preconditions = " << action->preconditiins() << ", effects = " << action->effects() << std::endl;
	}

	// create planner
	Planner* planner = new Planner;

	// Set up blackboard
	Blackboard* blackboard = new Blackboard;
	float health = 100.f;
	float enemyHealth = 100.f;
	float position = 30.f;
	float enemyPosition = 0.f;
	blackboard->registerEntry("health", &health);
	blackboard->registerEntry("enemyHealth", &enemyHealth);
	blackboard->registerEntry("position", &position);
	blackboard->registerEntry("enemyPosition", &enemyPosition);

	//// run planner
	//planner->plan(*initialState, *goalState, actions);
	//std::cin.get();

	// create runner
	Runner* runner = new Runner(actions, planner, initialState, goalState);

	// run
	time_point<system_clock> t = system_clock::now();
	int i = 0;
	while (true)
	{
		std::cout << "------------------------" << std::endl;
		if (i % 10 == 0)
		{
			i = 0;
			runner->tick(blackboard, true);
		}
		else
		{
			runner->tick(blackboard);
		}
		WorldStateInspector::onUpdate(blackboard, variableMap, runner->getCurrentState());
		t += milliseconds(1000);
		this_thread::sleep_until(t);
		i++;
	}

	return 0;
}