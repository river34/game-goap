//
//  WorldState.h
//  GameGOAP
//
//  Created by River Liu on 10/2/2018.
//  Copyright © 2018 River Liu. All rights reserved.
//

#pragma once

#include <string>
#include <map>
#include <ostream>

namespace GameGOAP
{
	class WorldState
	{
	private:
		std::string m_sName;
		std::map<int, bool> m_Variables;

	public:
		inline WorldState(const std::string& _name = "") : m_sName(_name) { }
		inline WorldState(const WorldState& _other) : m_sName(_other.m_sName), m_Variables(_other.m_Variables) { }
		inline const std::string& getName() { return m_sName; }
		inline void setVariable(const int _key, const bool _value) { m_Variables[_key] = _value; }
		inline bool getVariable(const int _key) const { return m_Variables.at(_key); }
		inline const std::map<int, bool>& getVariables() const { return m_Variables; }
		inline bool operator==(const WorldState& _other) const { return m_Variables == _other.m_Variables; }
		int distanceToState(const WorldState& _other) const
		{
			int distance = 0;
			for (const auto& var : _other.m_Variables)
			{
				auto it = m_Variables.find(var.first);
				if (it == m_Variables.end() || it->second != var.second)
				{
					distance++;
				}
			}
			return distance;
		}
		bool meetGoal(const WorldState& _goal) const
		{
			for (const auto& gVar : _goal.m_Variables) {
				try {
					if (m_Variables.at(gVar.first) != gVar.second) {
						return false;
					}
				}
				catch (const std::out_of_range&) {
					return false;
				}
			}
			return true;
		}
		friend std::ostream& operator<<(std::ostream& _out, const WorldState& _state)
		{
			_out << "WorldState { ";
			for (const auto& var : _state.m_Variables) {
				_out << var.second << " ";
			}
			_out << "}";
			return _out;
		}
	};
}
