#pragma once

#include "GameGOAPLib.h"

namespace GameGOAP
{
	class EnemyLostVariable : public Variable
	{
	public:
		EnemyLostVariable(int _key, bool _value) : Variable(_key, _value) { }
		EnemyLostVariable(int _key, bool _value, std::string& _name) :Variable(_key, _value, _name) { }
		EnemyLostVariable(const Params& _params) : Variable(_params) { }
		EnemyLostVariable(const EnemyLostVariable& _variable) : Variable(_variable) { }
		inline static Variable* create(const Params& _params) { return new EnemyLostVariable(_params); }
		void onUpdate(Blackboard* _blackboard)
		{
			std::cout << m_sName << " onUpdate" << std::endl;
		}
	};
}