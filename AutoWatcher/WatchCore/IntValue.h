#pragma once
#include "ValueBase.hpp"

class WATCHAPI Integer :
	public ValueBase <int>
{
public:
	Integer();
	Integer(const int& val);
	~Integer();
};

typedef  ValueBase<double> Real;

