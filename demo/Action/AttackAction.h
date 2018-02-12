#pragma once

#include "../../goap/Action.h"

using namespace GameGOAP;
class AttackAction : public Action
{
public:
	AttackAction(const Params& _params) : Action(_params) { }
	AttackAction(const AttackAction& _action) : Action(_action) { }
	inline static Action* create(const Params& _params) { return new AttackAction(_params); }
	void onUpdate(Blackboard* _blackboard)
	{
		std::cout << m_sName << " onUpdate" << std::endl;
	}
};