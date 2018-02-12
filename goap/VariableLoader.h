//
//  GOAPLoader.h
//  GameGOAP
//
//  Created by River Liu on 11/2/2018.
//  Copyright © 2018 River Liu. All rights reserved.
// 

#pragma once

#include <stdio.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include "Action.h"
#include "ActionFactory.h"
#include "WorldState.h"
#include "Params.h"
#include "Planner.h"
#include "Variable.h"
#include "../rapidxml/rapidxml.hpp"

using namespace rapidxml;

namespace GameGOAP
{
	class VariableLoader
	{
	public:
		void loaGOAP(const std::string& _file, ActionFactory* _actionFactory, std::vector<Action*>& _actions, std::vector<std::string>& _variableMap, WorldState& _initialState, WorldState& _goalState, const bool _isFilepath)
		{
			std::vector<char> buffer;

			if (_isFilepath == true)
			{
				std::ifstream theFile(_file);
				buffer = std::vector<char>((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
				std::cout << "[GOAPLoader] Read filepath" << std::endl;
			}
			else
			{
				buffer = std::vector<char>(_file.begin(), _file.end());
				std::cout << "[GOAPLoader] Read file as string" << std::endl;
			}
			buffer.push_back('\0');

			std::cout << "[GOAPLoader] File size: " << buffer.size() << std::endl;

			assert(buffer.size() > 1);

			// parse the buffer using the xml file parsing library into doc
			xml_document<> doc;
			doc.parse<0>(&buffer[0]);

			// find the root node
			xml_node<>* root_node = doc.first_node("GOAP");

			// find the ActionList node
			xml_node<>* actionList_node = root_node->first_node("ActionList");

			// iterate over actions
			for (xml_node<>* action_node = actionList_node->first_node("Action"); action_node; action_node = action_node->next_sibling("Action"))
			{
				// create action
				auto newAction = _actionFactory->createInstance(action_node);
				assert(newAction);
				std::cout << "[GOAPLoader] Create Action " << newAction->getName() << std::endl;

				// add preconditions
				for (xml_node<>* pre_node = action_node->first_node("Precondition"); pre_node; pre_node = pre_node->next_sibling("Precondition"))
				{
					Variable var = getVariable(pre_node, _variableMap);
					newAction->setPrecondition(var.m_iKey, var.m_bValue);
					std::cout << "  [GOAPLoader] Add Precondition. key = " << var.m_iKey << " value = " << var.m_bValue << std::endl;
				}

				// add effects
				for (xml_node<>* eff_node = action_node->first_node("Effect"); eff_node; eff_node = eff_node->next_sibling("Effect"))
				{
					Variable var = getVariable(eff_node, _variableMap);
					newAction->setEffect(var.m_iKey, var.m_bValue);
					std::cout << "  [GOAPLoader] Add Effect. key = " << var.m_iKey << " value = " << var.m_bValue << std::endl;
				}

				// add action to the action list
				_actions.push_back(newAction);
			}

			// find the InitialState node
			xml_node<>* initialState_node = root_node->first_node("InitialState");
			std::cout << "[GOAPLoader] Create Initial State " << std::endl;

			// add variables
			for (xml_node<>* var_node = initialState_node->first_node("Variable"); var_node; var_node = var_node->next_sibling("Variable"))
			{
				Variable var = getVariable(var_node, _variableMap);
				_initialState.setVariable(var.m_iKey, var.m_bValue);
				std::cout << "  [GOAPLoader] Add Variable. key = " << var.m_iKey << " value = " << var.m_bValue << std::endl;
			}

			// find the GoalState node
			xml_node<>* goalState_node = root_node->first_node("GoalState");
			std::cout << "[GOAPLoader] Create Goal State " << std::endl;

			// add variables
			for (xml_node<>* var_node = goalState_node->first_node("Variable"); var_node; var_node = var_node->next_sibling("Variable"))
			{
				Variable var = getVariable(var_node, _variableMap);
				_goalState.setVariable(var.m_iKey, var.m_bValue);
				std::cout << "  [GOAPLoader] Add Variable. key = " << var.m_iKey << " value = " << var.m_bValue << std::endl;
			}
		} // end of loaGOAP

		Variable getVariable(xml_node<>* _node, std::vector<std::string>& _variableMap)
		{
			assert(_node->first_attribute("name") != nullptr);
			assert(_node->first_attribute("value") != nullptr);
			std::string name = _node->first_attribute("name")->value();
			bool value = false;
			std::stringstream s(_node->first_attribute("value")->value());
			s >> std::boolalpha >> value;
			auto it = std::find(_variableMap.begin(), _variableMap.end(), name);
			if (it == _variableMap.end())
			{
				_variableMap.push_back(name);
			}
			it = std::find(_variableMap.begin(), _variableMap.end(), name);
			return Variable(std::distance(_variableMap.begin(), it), value);
		}
	};
}