#pragma once

#include "../../goap/Action.h"

using namespace GameGOAP;
class SearchAction : public Action
{
public:
	SearchAction(const Params& _params) : Action(_params) { }
	SearchAction(const SearchAction& _action) : Action(_action) { }
	inline static Action* create(const Params& _params) { return new SearchAction(_params); }
	void onUpdate(Blackboard* _blackboard)
	{
		std::cout << m_sName << " onUpdate" << std::endl;
	}
};