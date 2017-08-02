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
	Object() : _id(GenerateNullId()) {}
	virtual ~Object() {}
	virtual operator String  () const { return toString(); }
	ValuePtr operator [] (String name)
	{ 
		PropertyVector::iterator it = std::find_if(_properties.begin(), _properties.end(), [name](PropertyPtr p) {return p->name() == name; });
		if(it == _properties.end())
			throw NotFoundException(__func__);
		//Property search(name);
		//std::shared_ptr<Property> searchPtr(&search, [](Property*) {});
		//auto it = _properties.find(searchPtr);
		//if(it == _properties.end())
		//	throw NotImplementetException(__func__);
		PropertyPtr prop = *it;
		return prop->value();
	}

	PropertyVector& properties() { return _properties; }
	const PropertyVector& properties() const { return _properties; }

	static ObjectPtr make() { return std::make_shared<Object>(); }
	template <class PropertyListT>
	static ObjectPtr make(ObjectId id, PropertyListT p) { return std::make_shared<Object>(id, p); }
	template<class PropertyListT>
	static ObjectPtr make( PropertyListT p) { return std::make_shared<Object>(GenerateId(), p); }
 private:
	ObjectId _id;
	PropertyVector _properties;
};

//using ObjectValue = ValueImpl<ObjectPtr>;
//using ObjectValuePtr = std::shared_ptr<ObjectValue>;

