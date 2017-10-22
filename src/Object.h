#pragma once

#include "Thing.h"
#include "Property.h"
#include "Value.h"

namespace obj
{
class Object : public Thing
{
 public:
	Object() : _id(nullId){};
	Object(IdType id) : _id(id) {}
	template <class PropertyListT>
	Object(PropertyListT p) : _id(nullId), _properties(p.begin(), p.end()) {}
	template <class PropertyListT>
	Object(IdType id, PropertyListT p) : _id(id), _properties(p.begin(), p.end()) {}
	virtual ~Object() {}
	IdType id() const { return _id; }
	IdType &id() { return _id; }
	virtual bool operator<(const Thing & other) const override
	{
		if (auto o = dynamic_cast<decltype(this)>(&other))
			return *this < *o;
		else
			return Thing::operator<(other);
	}
	bool operator < (const Object& other) const;
	ValuePtr value(const String& name, ValuePtr default = ValuePtr());


 protected:
 public:
	virtual operator String() const { return toString(); }

	PropertyPtr property(const String &name)
	{
		auto it = std::find_if(_properties.begin(), _properties.end(), [name](PropertyPtr p) { return p->name() == name; });
		if (it == _properties.end())
			return PropertyPtr();
		else
			return *it;
	}

	ValuePtr at(const String &name)
	{
		PropertyPtr p = property(name);
		if (p)
			return p->value();
		else
			return Value::make();
	}

	PropertyVector &properties() { return _properties; }
	const PropertyVector &properties() const { return _properties; }

	static ObjectPtr make();
	static ObjectPtr make(IdType id);
	template <class PropertyListT>
	static ObjectPtr make(PropertyListT p);
	template <class PropertyListT>
	static ObjectPtr make(IdType id, PropertyListT p);

 private:
	IdType _id;
	PropertyVector _properties;
};

template <>
inline Result<ObjectPtr> &obj::Result<ObjectPtr>::operator=(ValuePtr value)
{
	PropertyPtr p = _container->property(_name);
	if (p)
		p->value() = value;
	else
		_container->properties().push_back(Property::make(_name, value));
	return *this;
}
template <>
inline Result<ObjectPtr> &obj::Result<ObjectPtr>::operator+=(ValuePtr value)
{
	VectorValuePtr v = *this;
	if (v)
		const_cast<ValuePtrVector&>(v->value()).push_back(value);
	else
		throw ImpossibleCastException(Format("'%1%' is not from type VectorValue") % typeid(*_result).name());
	return *this;
}
template<>
inline ValuePtr & Result<ObjectPtr>::operator[](size_t i)
{
	VectorValuePtr v = *this;
	if (v)
		return const_cast<ValuePtrVector&>(v->value()).at(i);
	throw ImpossibleCastException(Format("'%1%' is not from type VectorValue") % typeid(*_result).name());
}
inline Result<ObjectPtr> ObjectPtr::operator[](String name)
{	
	return Result<ObjectPtr>(name, *this, (*this)->at(name));
}
inline ObjectPtr Object::make() { return std::make_shared<Object>(); }
inline ObjectPtr Object::make(IdType id) { return std::make_shared<Object>(id); }

template <class PropertyListT>
inline ObjectPtr Object::make(PropertyListT p) { return std::make_shared<Object>(p); }
template <class PropertyListT>
inline ObjectPtr Object::make(IdType id, PropertyListT p) { return std::make_shared<Object>(id, p); }

template <>
String ObjectValue::toString() const { return _value->toString(); }

}
