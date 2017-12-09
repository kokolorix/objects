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
	ValuePtr value(const String& name, ValuePtr default = ValuePtr()) const;
	PropertyPtr property(const String &name) const;


 protected:
 public:
	virtual operator String() const { return toString(); }
	ValuePtr& findOrCreate(const String &name, ValuePtr default = Value::make());

	PropertyVector &properties() { return _properties; }
	const PropertyVector &properties() const { return _properties; }

	static ObjectPtr make();
	static ObjectPtr make(IdType id);
	template <class PropertyListT>
	static ObjectPtr make(PropertyListT p);
	template <class PropertyListT>
	static ObjectPtr make(IdType id, PropertyListT p);
	static ObjectPtr make(std::initializer_list<pair<String, ValuePtr>> r);

 private:
	IdType _id;
	PropertyVector _properties;
};

inline ObjectPtr Object::make() { return std::make_shared<Object>(); }
inline ObjectPtr Object::make(IdType id) { return std::make_shared<Object>(id); }

template <class PropertyListT>
inline ObjectPtr Object::make(PropertyListT p) { return std::make_shared<Object>(p); }
template <class PropertyListT>
inline ObjectPtr Object::make(IdType id, PropertyListT p) { return std::make_shared<Object>(id, p); }

template <>
String ObjectValue::toString() const { return _value->toString(); }

}
