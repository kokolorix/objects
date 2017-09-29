#include "stdafx.h"
#include "Property.h"

inline bool obj::Property::operator<(const Property & o)
{
	return std::tie(_name, _value) < std::tie(o._name, o._value);
}
