//
//  Variable.h
//  GameGOAP
//
//  Created by River Liu on 11/2/2018.
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
	class Variable
	{
	protected:
		int m_iKey;
		bool m_bValue;
		std::string m_sName;

	public:
		typedef Variable* (*createInstanceFn) (const Params&);

	public:
		Variable() {}
		Variable(int _key, bool _value) : m_iKey(_key), m_bValue(_value), m_sName("Variable") { }
		Variable(int _key, bool _value, std::string& _name) : m_iKey(_key), m_bValue(_value), m_sName(_name) { }
		Variable(const Params& _params)
		{
			m_iKey = 0;
			auto it = _params.find("key");
			if (it != _params.end())
			{
				std::stringstream s(it->second);
				s >> m_iKey;
			}
			m_bValue = false;
			m_sName = "Variable";
			it = _params.find("name");
			if (it != _params.end())
			{
				m_sName = it->second;
			}
		}
		inline const std::string& getName() const { return m_sName; }
		inline const bool getValue() const { return m_bValue; }
		void virtual onUpdate(Blackboard* _blackboard) { std::cout << "wrong place" << std::endl; }
		inline static Variable* create(const Params& _params) { return new Variable(_params); }
		friend class GOAPLoader;
		friend class WorldState;
	};
}
