#pragma once
#include "Define.h"
#include <float.h>
#include <limits.h>

template <class ValueType>
class ValueBase
{
public:
	ValueType& Value(void) 
	{
		return _value;
	}

	void Value(const ValueType& val) 
	{ 
		SetValue(val);
	}

	ValueType* Reference(void)
	{
		return &_value;
	}

	operator ValueType()
	{
		return _value;
	}

	ValueType& operator=(const ValueType& val)
	{
		SetValue(val);
		return _value;
	}

protected:
	ValueBase(const ValueType& val)
		:_value(val)
	{
	}

	ValueBase(ValueBase& orginal)
		:_value(orginal)
	{
	}

private:
	void SetValue(const ValueType& val)
	{
		Checker<ValueType> checker;
		if (checker.Check(val))
		{
			_value = val;
		}
		else
		{
			Logger::Log("无法通过数值有效性验证，放弃赋值", Warning);
		}
	}

	ValueType _value;
};

template <class ValueType>
class Checker
{
public:
	bool Check(const ValueType& val)
	{
		return true;
	}
};

template <>
class Checker <double>
{
public:
	bool Check(const double& val)
	{
		return (val > DBL_MIN && val < DBL_MAX);
	}
};

template <>
class Checker <int>
{
public:
	bool Check(const int& val)
	{
		return (val > INT_MIN && val < INT_MAX);
	}
};