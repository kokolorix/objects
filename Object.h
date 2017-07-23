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
	Object(ObjectId id, PropertyVector p) : _id(id), _properties(p.begin(), p.end()) {}
	Object(PropertyVector p) : Object(GenerateId(), p) {}
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

 private:
	ObjectId _id;
	PropertySet _properties;
};

using ObjectValue = ValueImpl<ObjectPtr>;
using ObjectValuePtr = std::shared_ptr<ObjectValue>;

