#include "stdafx.h"
#include "Object.h"
using namespace obj;

bool obj::operator < (ObjectPtr x, ObjectPtr y)
{
	bool result = (x.get() && y.get()) ? (*x < *y) : x.get() < y.get();
	return result;
}
bool obj::operator == (ObjectPtr x, ObjectPtr y)
{
	bool result = x.get() == y.get() || (x.get() && y.get() && !(*x < *y) && !(*y < *x));
	return result;
}

bool obj::Object::operator<(const Object & o) const
{
	if (_id && o._id)
		return std::tie(_id, _properties) < std::tie(o._id, o._properties);
	else
		return _properties < o._properties;
}

ValuePtr obj::Object::value(const String & name, ValuePtr default)
{
	return default;
}

ObjectPtr& ObjectPtr::operator+=(PropertyPtr p)
{
	(*this)->properties().push_back(p);
	return *this;
}

