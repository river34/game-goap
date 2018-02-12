//
//  Action.h
//  GameGOAP
//
//  Created by River Liu on 10/2/2018.
//  Copyright © 2018 River Liu. All rights reserved.
//

#pragma once

#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include "WorldState.h"
#include "Params.h"

class Blackboard;
namespace GameGOAP
{
	class Action
	{
	protected:
		std::string m_sName;
		int m_iCost;
		std::map<int, bool> m_Preconditions;
		std::map<int, bool> m_Effects;

	public:
		typedef Action* (*createInstanceFn) (const Params&);

	public:
		inline Action() : m_sName("Action"), m_iCost(0) { }
		inline Action(const std::string& _name, const int _cost) : m_sName(_name), m_iCost(_cost) { }
		Action(const Params& _params)
		{
			m_sName = "Action";
			auto it = _params.find("name");
			if (it != _params.end())
			{
				m_sName = it->second;
			}
			m_iCost = 1;
			it = _params.find("cost");
			if (it != _params.end())
			{
				std::stringstream s(it->second);
				s >> m_iCost;
			}
		}
		inline const std::string& getName() const { return m_sName; }
		inline const int preconditiins() const { return m_Preconditions.size(); }
		inline const int effects() const { return m_Effects.size(); }
		inline void setPrecondition(const int _key, const bool _value) { m_Preconditions[_key] = _value; }
		inline void setEffect(const int _key, const bool _value) { m_Effects[_key] = _value; }
		bool isValid(const WorldState& _state) const
		{
			for (const auto& pre : m_Preconditions)
			{
				if (pre.second != _state.getVariable(pre.first))
				{
					return false;
				}
			}
			return true;
		}
		WorldState proceed(const WorldState& _state) const
		{
			WorldState nextState(_state);
			//std::cout << "  [" << m_sName << "] " << "m_Effects = " << m_Effects.size() << std::endl;
			for (const auto& effect : m_Effects)
			{
				nextState.setVariable(effect.first, effect.second);
				//std::cout << "  [" << m_sName << "] " << effect.first << " = " << effect.second << std::endl;
			}
			//std::cout << "  [" << m_sName << "] " << nextState << std::endl;
			return nextState;
		}
		inline static Action* create(const Params& _params) { return new Action(_params); }
		inline virtual void onUpdate(Blackboard* _blackboard) { }
		friend class Planner;
	};
}
