//
//  VariableFactory.h
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
#include "Variable.h"
#include "../rapidxml/rapidxml.hpp"

namespace GameGOAP
{
	class VariableFactory
	{
	protected:
		int m_iKey;
		bool m_bValue;

	public:
		VariableFactory(const VariableFactory&) {}
		VariableFactory& operator=(const VariableFactory&) { return *this; }
		typedef std::map<std::string, Variable::createInstanceFn> FactoryMap;
		FactoryMap m_FactoryMap;

	public:
		VariableFactory() { }
		~VariableFactory() { m_FactoryMap.clear(); }
		void registerClass(const std::string& name, Variable::createInstanceFn pfnCreate)
		{
			m_FactoryMap[name] = pfnCreate;
		}
		Variable* createInstance(const rapidxml::xml_node<>* node)
		{
			std::string id = (node->first_attribute("id") == nullptr) ? "" : node->first_attribute("id")->value();
			std::string name = node->name();
			//std::cout << "[VariableFactory] to find " << id + name << std::endl;
			FactoryMap::iterator it = m_FactoryMap.find(id + name);
			Params params = Params();
			for (rapidxml::xml_attribute<>* attr = node->first_attribute(); attr; attr = attr->next_attribute())
			{
				params[attr->name()] = attr->value();
				params["name"] = id + name;
				//std::cout << "[VariableFactory] param " << attr->name() << " = " << attr->value() << std::endl;
			}
			if (it != m_FactoryMap.end())
			{
				Variable* variable = it->second(params);
				//std::cout << "[VariableFactory] created " << action->getName() << std::endl;
				return variable;
			}
			return nullptr;
		}
	};
}
