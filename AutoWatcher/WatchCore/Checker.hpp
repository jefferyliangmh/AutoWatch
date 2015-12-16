// ***********************************************************************
// Assembly         : 
// Author           : 梁明辉
// Created          : 11-18-2015
//
// Last Modified By : 梁明辉
// Last Modified On : 11-18-2015
// ***********************************************************************
// <copyright file="Checker.hpp" company="LC 软件工作室">
//     Copyright (c) LC 软件工作室. All rights reserved.
// </copyright>
// <summary></summary>
// ***********************************************************************
#pragma once
#include <float.h>
#include <limits.h>

template <class ValueType>
class Checker
{
public:
	static bool Check(const ValueType& val)
	{
		return true;
	}
};

//浮点数据类型
template <>
class Checker <double>
{
public:
	static bool Check(const double& val)
	{
		return (val > DBL_MIN && val < DBL_MAX);
	}
};

//整型数据类型
template <>
class Checker <char>
{
public:
	static bool Check(const char& val)
	{
		return (val > CHAR_MIN && val < CHAR_MAX);
	}
};

template <>
class Checker <unsigned char>
{
public:
	static bool Check(const unsigned char& val)
	{
		return (val < UCHAR_MAX);
	}
};

template <>
class Checker <short>
{
public:
	static bool Check(const short& val)
	{
		return (val > SHRT_MIN && val < SHRT_MAX);
	}
};

template <>
class Checker <unsigned short>
{
public:
	static bool Check(const unsigned short& val)
	{
		return (val < USHRT_MAX);
	}
};

template <>
class Checker <int>
{
public:
	static bool Check(const int& val)
	{
		return (val > INT_MIN && val < INT_MAX);
	}
};

template <>
class Checker <unsigned int>
{
public:
	static bool Check(const unsigned int& val)
	{
		return (val < UINT_MAX);
	}
};

template <>
class Checker <long>
{
public:
	static bool Check(const long& val)
	{
		return (val > LONG_MIN && val < LONG_MAX);
	}
};

template <>
class Checker <unsigned long>
{
public:
	static bool Check(const unsigned long& val)
	{
		return (val < ULONG_MAX);
	}
};

template <>
class Checker <long long>
{
public:
	static bool Check(const long long& val)
	{
		return (val > LLONG_MIN && val < LLONG_MAX);
	}
};

template <>
class Checker <unsigned long long>
{
public:
	static bool Check(const unsigned long long& val)
	{
		return (val < ULLONG_MAX);
	}
};