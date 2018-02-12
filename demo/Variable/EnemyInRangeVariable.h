#pragma once

#include "GameGOAPLib.h"

namespace GameGOAP
{
	class EnemyInRangeVariable : public Variable
	{
	private:
		float m_fLessEqualThan;

	public:
		EnemyInRangeVariable(int _key, bool _value) : m_fLessEqualThan(0.f), Variable (_key, _value) { }
		EnemyInRangeVariable(int _key, bool _value, std::string& _name) : m_fLessEqualThan(0.f), Variable(_key, _value, _name) { }
		EnemyInRangeVariable(const Params& _params)
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
			m_fLessEqualThan = 0;
			it = _params.find("let");
			if (it != _params.end())
			{
				std::stringstream s(it->second);
				s >> m_fLessEqualThan;
			}
		}
		EnemyInRangeVariable(const EnemyInRangeVariable& _variable) : Variable(_variable) { }
		inline static Variable* create(const Params& _params) { return new EnemyInRangeVariable(_params); }
		void onUpdate(Blackboard* _blackboard)
		{
			std::cout << m_sName << " onUpdate" << std::endl;
		}
	};
}