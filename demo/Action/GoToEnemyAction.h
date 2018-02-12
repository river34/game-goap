#pragma once

#include "../../goap/Action.h"

using namespace GameGOAP;
class GoToEnemyAction : public Action
{
public:
	GoToEnemyAction(const Params& _params) : Action(_params) { }
	GoToEnemyAction(const GoToEnemyAction& _action) : Action(_action) { }
	inline static Action* create(const Params& _params) { return new GoToEnemyAction(_params); }
	void onUpdate(Blackboard* _blackboard)
	{
		std::cout << m_sName << " onUpdate" << std::endl;
	}
};