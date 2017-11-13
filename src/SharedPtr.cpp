#include "stdafx.h"
#include "SharedPtr.h"
#include "Value.h"
#include "Property.h"
#include "Object.h"
#include "Xml.h"

using namespace obj;

Result Result::operator=(ValuePtr value)
{
	PropertyPtr p = _object->property(_name);
	if (p)
		p->value() = value;
	else
		_object->properties().push_back(Property::make(_name, value));
	return *this;
}

Result Result::operator =(std::initializer_list<std::initializer_list<pair<String, ValuePtr>>> il)
{
	ValuePtrVector v;
	for (auto& ol : il)
		v.push_back(Object::make(ol));
	return (*this) = Value::make(std::move(v));
	//return *this;
}

Result Result::operator+=(ValuePtr value)
{
	VectorValuePtr v = *this;
	if (v)
		const_cast<ValuePtrVector&>(v->value()).push_back(value);
	else
		throw ImpossibleCastException(Format("'%1%' is not from type VectorValue") % typeid(*_result).name());
	return *this;
}

ValuePtr & Result::operator[](size_t i)
{
	VectorValuePtr v = *this;
	if (v)
		return const_cast<ValuePtrVector&>(v->value()).at(i);
	throw ImpossibleCastException(Format("'%1%' is not from type VectorValue") % typeid(*_result).name());
}

Result Result::operator[](const String & name)
{
	return Result(name, _object, _object->findOrCreate(name));
}

Result ObjectPtr::operator[](const String& name)
{
	return Result(name, *this, (*this)->findOrCreate(name));
}

Result obj::ObjectPtr::operator=(ValuePtr value)
{
	return Result(xml::TEXT_NODE, *this, (*this)->findOrCreate(xml::TEXT_NODE));
}
