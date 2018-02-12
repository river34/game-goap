//
//  ActionFactory.h
//  GameGOAP
//
//  Created by River Liu on 11/2/2018.
//  Copyright © 2018 River Liu. All rights reserved.
//

#pragma once

#include <tuple>
#include <iostream>
#include <string>
#include <map>
#include "Action.h"
#include "../rapidxml/rapidxml.hpp"

namespace GameGOAP
{
	class ActionFactory
	{
	private:
		ActionFactory(const ActionFactory&) {}
		ActionFactory& operator=(const ActionFactory&) { return *this; }
		typedef std::map<std::string, Action::createInstanceFn> FactoryMap;
		FactoryMap m_FactoryMap;

	public:
		ActionFactory() { }
		~ActionFactory() { m_FactoryMap.clear(); }
		void registerClass(const std::string& name, Action::createInstanceFn pfnCreate)
		{
			m_FactoryMap[name] = pfnCreate;
		}
		Action* createInstance(const rapidxml::xml_node<>* node)
		{
			std::string id = (node->first_attribute("id") == nullptr) ? "" : node->first_attribute("id")->value();
			std::string name = node->name();
			//std::cout << "[ActionFactory] to find " << id + name << std::endl;
			FactoryMap::iterator it = m_FactoryMap.find(id + name);
			Params params = Params();
			for (rapidxml::xml_attribute<>* attr = node->first_attribute(); attr; attr = attr->next_attribute())
			{
				params[attr->name()] = attr->value();
				params["name"] = id + name;
				//std::cout << "[ActionFactory] param " << attr->name() << " = " << attr->value() << std::endl;
			}
			if (it != m_FactoryMap.end())
			{
				Action* action = it->second(params);
				//std::cout << "[ActionFactory] created " << action->getName() << std::endl;
				return action;
			}
			return nullptr;
		}
	};
}