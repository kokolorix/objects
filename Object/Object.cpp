#include "stdafx.h"
#include "Object.h"
using namespace obj;

obj::ObjectPtr::ObjectPtr(std::initializer_list<pair<String, ValuePtr>> r)
	: Base(Object::make(r))
{
}

ObjectPtr obj::Object::make(std::initializer_list<pair<String, ValuePtr>> r)
{
	ObjectPtr obj = Object::make();
	for (auto p : r)
		obj[p.first] = p.second;
	return obj;
}

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

ValuePtr obj::Object::value(const String & name, ValuePtr default) const
{
	PropertyPtr p = property(name);
	if (p)
		p->value();
	return default;
}

PropertyPtr obj::Object::property(const String & name) const
{
	auto it = std::find_if(_properties.begin(), _properties.end(), [name](PropertyPtr p) { return p->name() == name; });
	if (it == _properties.end())
		return PropertyPtr();
	else
		return *it;
}

ObjectPtr & obj::ObjectPtr::operator()(const String & name)
{
	ObjectValuePtr v = dynamic_pointer_cast<ObjectValue>((*this)->findOrCreate(name, Object::make()));
	return const_cast<ObjectPtr&>(v->value());
}

ObjectPtr& ObjectPtr::operator+=(PropertyPtr p)
{
	(*this)->properties().push_back(p);
	return *this;
}

ValuePtr & obj::Object::findOrCreate(const String & name, ValuePtr default)
{
	PropertyPtr p = property(name);
	if (!p)
	{
		p = Property::make(name, default);
		_properties.push_back(p);
	}
	return p->value();
} 
