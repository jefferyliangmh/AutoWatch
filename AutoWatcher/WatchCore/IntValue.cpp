#include "stdafx.h"
#include "IntValue.h"
#include "Default.hpp"

Integer::Integer()
	:ValueBase(Default::IntegerDefault)
{
}

Integer::Integer(const int& val)
	:ValueBase(val)
{
}

Integer::~Integer()
{
}