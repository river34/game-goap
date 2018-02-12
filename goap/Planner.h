//
//  Planner.h
//  GameGOAP
//
//  Created by River Liu on 10/2/2018.
//  Copyright © 2018 River Liu. All rights reserved.
//

#pragma once

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>
#include "WorldState.h"
#include "Action.h"
#include "Node.h"

namespace GameGOAP
{
#ifndef MAX_ACTIONS
#define MAX_ACTIONS 16
#define MAX_VARIABLES 16
#endif
	class Planner
	{
	private:
		std::vector<Node> m_OpenList;   // The A* open list
		std::vector<Node> m_ClosedList; // The A* closed list

	private:
		std::vector<Node>::iterator inOpenList(const WorldState& _state)
		{
			return std::find_if(m_OpenList.begin(), m_OpenList.end(), [&](const Node & node) { return node.m_WorldState == _state; });
		}
		bool inClosedList(const WorldState& _state)
		{
			if (std::find_if(m_ClosedList.begin(), m_ClosedList.end(), [&](const Node & node){ return node.m_WorldState == _state; }) == m_ClosedList.end())
			{
				return false;
			}
			return true;
		}
		Node& popAndClose()
		{
			assert(m_OpenList.empty() == false);
			m_ClosedList.push_back(m_OpenList.front());
			m_OpenList.erase(m_OpenList.begin());
			return m_ClosedList.back();
		}
		inline void addToOpenList(const Node& _node)
		{
			auto it = std::lower_bound(m_OpenList.begin(), m_OpenList.end(), _node);
			m_OpenList.emplace(it, std::move(_node));
		}
		inline int getHeuristic(const WorldState& _state1, const WorldState& _state2) const { return _state1.distanceToState(_state2); }
		
	public:
		inline Planner() {}
		inline void printOpenList() const { for (const auto& n : m_OpenList) { std::cout << n << std::endl; } }
		inline void printClosedList() const { for (const auto& n : m_ClosedList) { std::cout << n << std::endl; } }
		std::vector<Action*> plan(const WorldState& _start, const WorldState& _goal, const std::vector<Action*>& _actions)
		{
			// if start and end states are the same
			if (_start.meetGoal(_goal))
			{
				return std::vector<Action*>();
			}

			// clear list if not
			Node::clear();
			m_OpenList.clear();
			m_ClosedList.clear();

			// push start state to the open list
			Node start(_start, 0, getHeuristic(_start, _goal), 0, nullptr);
			addToOpenList(start);

			// valid if the open list is not empty
			int iter = 0;
			while (m_OpenList.size() > 0)
			{
				++iter;
				std::cout << "\n-----------------------\n";
				std::cout << "Iteration " << iter << std::endl;

				// remove the lowest rank (F) state from the open list, add it to the closed list
				Node& current(popAndClose());

				std::cout << "Open list: " << std::endl;
				printOpenList();
				std::cout << "Closed list: " << std::endl;
				printClosedList();
				std::cout << "Current: " << current << std::endl;

				std::cout << "  Current world state: " << current.m_WorldState << std::endl;
				std::cout << "  Goal world state: " << _goal << std::endl;

				// if current state and end states are the same, find a path
				if (current.m_WorldState.meetGoal(_goal))
				{
					std::cout << "Find a path!" << std::endl;
					std::vector<Action*> plan;
					do
					{
						plan.push_back(current.m_pAction);
						auto it = std::find_if(m_OpenList.begin(), m_OpenList.end(), [&](const Node & node) { return node.m_iID == current.m_iParentID; });
						if (it == m_OpenList.end())
						{
							it = std::find_if(m_ClosedList.begin(), m_ClosedList.end(), [&](const Node & node) { return node.m_iID == current.m_iParentID; });
						}
						current = *it;
					}
					while (current.m_iParentID != 0);
					std::cout << "Plan: ";
					for (const Action* action : plan)
					{
						std::cout << action->getName() << ", ";
					}
					std::cout << std::endl;
					return plan;
				}

				// check each neighbour of the current state
				for (const auto& action : _actions)
				{
					if (action->isValid(current.m_WorldState))
					{
						std::cout << action->m_sName <<  " is valid " << std::endl;

						WorldState out = action->proceed(current.m_WorldState);

						std::cout << "Out: " << out << std::endl;

						// if neighbour is in the closed list
						if (inClosedList(out))
						{
							std::cout << "neighbour is in the closed list " << std::endl;
							continue;
						}

						std::cout << action->m_sName << " will go to " << out << std::endl;

						// if neighbour is not in the open list, add neighbour to the open list
						auto next = inOpenList(out);
						if (next == m_OpenList.end())
						{
							Node next(out, current.m_iG + action->m_iCost, getHeuristic(out, _goal), current.m_iID, action);
							addToOpenList(std::move(next));
						}
						// if neighbour is in the open list
						else
						{
							// if cost is less than G(neighbour)
							if (current.m_iG + action->m_iCost < next->m_iG)
							{
								std::cout << "Path to " << next->m_WorldState << " using " << action->m_sName << " (combined cost " <<
									current.m_iG + action->m_iCost << ") is better than existing (cost " << next->m_iG << "\n";
								next->m_iParentID = current.m_iID;
								next->m_iG = current.m_iG + action->m_iCost;
								next->m_iH = getHeuristic(out, _goal);
								next->m_pAction = action;

								// resort open list to account for the new F
								std::sort(m_OpenList.begin(), m_OpenList.end());
							}
						}
					}
				}
			}

			// If there's nothing left to evaluate, then we have no possible path left
			throw std::runtime_error("A* planner could not find a path from start to goal");
		} // end of Planned::plan
	};
}