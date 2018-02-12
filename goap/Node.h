// 
//  Node.h
//  GameGOAP
//
//  Created by River Liu on 10/2/2018.
//  Copyright © 2018 River Liu. All rights reserved.
//

#pragma once

#include <ostream>
#include "WorldState.h"
#include "Action.h"

namespace GameGOAP
{
	class Node
	{
	private:
		static int lastID;

	private:
		WorldState m_WorldState;	// each node wraps a world state
		int m_iID;			// unique id
		int m_iParentID;	// unique id of its predecessor
		int m_iG;	// g value
		int m_iH;	// h value
		Action* m_pAction;	// action from predecessor to get here for replay

	public:
		inline Node() : m_iG(0), m_iH(0) { m_iID = lastID++; }
		inline Node(const WorldState _state, int _g, int _h, int _parentID, Action* _action) : m_WorldState(_state), m_iG(_g), m_iH(_h), m_iParentID(_parentID), m_pAction(_action) { m_iID = ++lastID; }
		inline int f() const { return m_iG + m_iH; }
		inline bool operator<(const Node& _other) { return f() < _other.f(); }
		friend std::ostream& operator<<(std::ostream& _out, const Node& _node)
		{
			_out << "Node { id:" << _node.m_iID << ", parent: " << _node.m_iParentID << ", F: " << _node.f() << ", G: " << _node.m_iG << ", H: " << _node.m_iH;
			_out << ", " << _node.m_WorldState << "}";
			return _out;
		}
		inline static void clear() { lastID = 0; }
		friend class Planner;
	};
}