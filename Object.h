#pragma once

#include "Thing.h"
#include "Property.h"

class Object;
using ObjectPtr = std::shared_ptr<Object>;

using ObjectId = boost::uuids::uuid;
using ObjectTypeId = boost::uuids::uuid;

class Object : public Thing
{
 public:
	template<class PropertyListT>
	Object(ObjectId id, PropertyListT p) : _id(id), _properties(p.begin(), p.end()) {}
	template<class PropertyListT>
	Object(PropertyListT p) : Object(GenerateId(), p) {}
	virtual ~Object() {}
	virtual ValuePtr operator [] (String name)
	{ 
		Property search(name);
		std::shared_ptr<Property> searchPtr(&search, [](Property*) {});
		auto it = _properties.find(searchPtr);
		if(it == _properties.end())
			throw NotImplementetException(__func__);
		PropertyPtr prop = *it;
		return std::dynamic_pointer_cast<Value>(prop->value());
	}

	template<class PropertyListT>
	static ObjectPtr make(ObjectId id, PropertyListT p) { return std::make_shared<Object>(id, p); }
	template<class PropertyListT>
	static ObjectPtr make( PropertyListT p) { return std::make_shared<Object>(GenerateId(), p); }
 private:
	ObjectId _id;
	PropertySet _properties;
};

using ObjectValue = ValueImpl<ObjectPtr>;
using ObjectValuePtr = std::shared_ptr<ObjectValue>;

