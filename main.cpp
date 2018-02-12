#include "GameGOAPLib.h"
#include "demo/Action/AttackAction.h"
#include "demo/Action/GoToEnemyAction.h"
#include "demo/Action/SearchAction.h"
#include "demo/Variable/EnemyDeadVariable.h"
#include "demo/Variable/EnemyInRangeVariable.h"
#include "demo/Variable/EnemyLostVariable.h"
#include "Blackboard.h"

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

using namespace GameGOAP;
using namespace std;
using namespace std::chrono;

// reset node
int GameGOAP::Node::lastID = 0;

int main(void)
{
	std::cout << "Example running...\n";

	// create action list
	std::vector<Action*> actions;
	actions.reserve(MAX_ACTIONS);

	// create variable list
	std::vector<Variable*> variables;
	actions.reserve(MAX_VARIABLES);

	// create variable map
	std::vector<std::string> variableMap;

	// create initial and goal states
	WorldState* initialState = new WorldState;
	WorldState* goalState = new WorldState;

	// create factory
	ActionFactory* actionFactory = new ActionFactory;
	actionFactory->registerClass("AttackAction", &AttackAction::create);
	actionFactory->registerClass("GoToEnemyAction", &GoToEnemyAction::create);
	actionFactory->registerClass("SearchAction", &SearchAction::create);
	VariableFactory* variableFactory = new VariableFactory;
	variableFactory->registerClass("EnemyDeadVariable", &EnemyDeadVariable::create);
	variableFactory->registerClass("EnemyInRangeVariable", &EnemyInRangeVariable::create);
	variableFactory->registerClass("EnemyLostVariable", &EnemyLostVariable::create);
	
	// load from file
	GOAPLoader* loader = new GOAPLoader;
	loader->loaGOAP("GOAPTest1.xml", actionFactory, actions, variableFactory, variables, *initialState, *goalState, true);

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
		t += milliseconds(1000);
		this_thread::sleep_until(t);
		i++;
	}

	return 0;
}