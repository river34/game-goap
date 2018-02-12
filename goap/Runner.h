//
//  Runner.h
//  GameGOAP
//
//  Created by River Liu on 11/2/2018.
//  Copyright © 2018 River Liu. All rights reserved.
//

#pragma once

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>
#include "Action.h"
#include "Planner.h"

namespace GameGOAP
{
	class Runner
	{
	private:
		std::vector<Action*> m_Path; // Active path
		std::vector<Action*> m_ActionList; // Active list
		Planner* m_Planner;
		WorldState* m_InitialState;
		WorldState* m_GoalState;
		Action* m_CurrentAction;

	public:
		Runner(std::vector<Action*> _actionList, Planner* _planner, WorldState* _initialState, WorldState* _goalState) :
			m_ActionList(_actionList), m_Planner(_planner), m_InitialState(_initialState), m_GoalState(_goalState) { m_CurrentAction = nullptr; }
		void tick(Blackboard* _blackboard, bool forcePlan = false)
		{
			//std::cout << "tick Runner" << std::endl;
			//if (m_Path.empty()) std::cout << "m_Path is empty" << std::endl;
			//else std::cout << "m_Path is not empty" << std::endl;
			//if (m_ActionList.empty()) std::cout << "m_ActionList is empty" << std::endl;
			//else std::cout << "m_ActionList is not empty" << std::endl;
			//if (m_Planner == nullptr) std::cout << "m_Planner == nullptr" << std::endl;
			//else std::cout << "m_Planner != nullptr" << std::endl;
			//if (m_InitialState == nullptr) std::cout << "m_InitialState == nullptr" << std::endl;
			//else std::cout << "m_InitialState != nullptr" << std::endl;
			//if (m_GoalState == nullptr) std::cout << "m_GoalState == nullptr" << std::endl;
			//else std::cout << "m_GoalState != nullptr" << std::endl;
			//if (m_CurrentAction == nullptr) std::cout << "m_CurrentAction == nullptr" << std::endl;
			//else std::cout << "m_CurrentAction != nullptr" << std::endl;

			if (forcePlan == false)
			{
				if (m_CurrentAction == nullptr)
				{
					if (m_Path.empty() == true)
					{
						m_Path = m_Planner->plan(*m_InitialState, *m_GoalState, m_ActionList);
					}

					if (m_Path.empty() == false)
					{
						m_CurrentAction = m_Path.back();
						m_Path.pop_back();
					}
				}
			}
			else
			{
				m_Path = m_Planner->plan(*m_InitialState, *m_GoalState, m_ActionList);
				if (m_Path.empty() == false)
				{
					m_CurrentAction = m_Path.back();
					m_Path.pop_back();
				}
			}
			if (m_CurrentAction != nullptr)
			{
				m_CurrentAction->onUpdate(_blackboard);
			}
		}
		WorldState& getCurrentState() { return *m_InitialState; }
	};
}