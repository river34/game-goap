#pragma once

#include "../GameGOAPLib.h"
#include "../Blackboard.h"

using namespace GameGOAP;

class WorldStateInspector
{
public:
	static void onUpdate(const Blackboard* _blackboard, const std::vector<std::string>& _variableMap, WorldState& _currState)
	{
		for (auto name : _variableMap)
		{
			ptrdiff_t key = std::find(_variableMap.begin(), _variableMap.end(), name) - _variableMap.begin();
			if (key >= _variableMap.size()) continue;

			if (name == "EnemyInRange")
			{
				_currState.setVariable(key, true);
			}
		}
	}
};