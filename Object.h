#pragma once

#include "Thing.h"
#include "Property.h"

namespace obj
{
class Object : public Thing
{
 protected:
	 Object() {};
	template <class PropertyListT>
	Object(PropertyListT p) : _properties(p.begin(), p.end()) {}
	virtual ~Object() {}
	 friend class std::_Ref_count_obj<Object>;

 public:
	virtual operator String() const { return toString(); }

	ValuePtr at(const String &name)
	{
		PropertyVector::iterator it = std::find_if(_properties.begin(), _properties.end(), [name](PropertyPtr p) { return p->name() == name; });
		if (it == _properties.end())
			return NothingValue<Unknown>::make();
		PropertyPtr prop = *it;
		return prop->value();
	}

	PropertyVector &properties() { return _properties; }
	const PropertyVector &properties() const { return _properties; }

	static ObjectPtr make();
	template <class PropertyListT>
	static ObjectPtr make(PropertyListT p);

 private:
	PropertyVector _properties;
};

inline ValuePtr ObjectPtr::operator[](String name) { return (*this)->at(name); }

template <typename IdT = UuId>
class ObjectImpl : public Object
{
 public:
	ObjectImpl() : Object({"id", generateNullId()})
	{
	}
	template <class PropertyListT>
	ObjectImpl(PropertyListT p) : Object(p)
	{
	}
	template <class PropertyListT>
	ObjectImpl(const IdT &id, PropertyListT p) : Object(p) {}
};

inline ObjectPtr Object::make()
{
	return std::make_shared<Object>();
}

template <class PropertyListT>
inline ObjectPtr Object::make(PropertyListT p)
{
	return std::make_shared<Object>(p);
}
}
