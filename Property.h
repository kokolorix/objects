#pragma once

#include "Thing.h"
#include "Value.h"
#include <set>
#include <vector>

class Property;
using PropertyPtr = std::shared_ptr<Property>;

class Property : public Thing
{
 public:
	Property(String n, ThingPtr v = ThingPtr()) : _name(n), _value(v) {}
	virtual ~Property() {}
	String name() const { return _name; }
	ThingPtr value() const{ return _value; }
	static PropertyPtr make(String n, ThingPtr v = ThingPtr()) { return std::make_shared<Property>(n, v); }

private:
	String _name;
	ThingPtr _value;
};

struct LessProperty
{
	bool operator () (PropertyPtr x, PropertyPtr y) const
	{
		return x->name() < y->name();
	}
};
using PropertySet = std::set<PropertyPtr, LessProperty>;
using PropertyVector = std::vector<PropertyPtr>;	
