#include "stdafx.h"
#include "Property.h"

obj::PropertyPtr::PropertyPtr(pair<String, ValuePtr> p)
	: Base(Property::make(p.first, p.second))
{
}

bool obj::Property::operator<(const Property & o) const
{
	if (_id && o._id)
		return std::tie(_id, _name, _value) < std::tie(o._id, o._name, o._value);
	else
		return std::tie(_name, _value) < std::tie(o._name, o._value);
}
