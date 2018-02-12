//
//  Variable.h
//  GameGOAP
//
//  Created by River Liu on 11/2/2018.
//  Copyright © 2018 River Liu. All rights reserved.
// 

#pragma once

namespace GameGOAP
{
	class Variable
	{
	private:
		int m_iKey;
		bool m_bValue;
	public:
		Variable(int _key, bool _value) : m_iKey(_key), m_bValue(_value) { }
		friend class GOAPLoader;
	};
}
