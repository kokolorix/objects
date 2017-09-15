#pragma once

#include "Thing.h"
#include "Value.h"
#include <set>
#include <vector>

namespace obj
{
using ::std::set;
using ::std::vector;
}

namespace obj
{
class Property;
using PropertyPtr = shared_ptr<Property>;

class Property : public Thing
{
public:
	Property(String n, ValuePtr v = ValuePtr()) :_id(nullId), _name(n), _value(v) {}
	Property(IdType id, String n, ValuePtr v = ValuePtr()) :_id(id), _name(n), _value(v) {}
	Property() {}
	virtual ~Property() {}
	IdType id() const { return _id; }
	IdType& id() { return _id; }
	virtual operator String  () const { return toString(); }
	const String& name() const { return _name; }
	String& name() { return _name; }
	ValuePtr value() const { return _value; }
	ValuePtr& value() { return _value; }
	static PropertyPtr make(String n, ValuePtr v = ValuePtr()) { return std::make_shared<Property>(n, v); }
	static PropertyPtr make(IdType id, String n, ValuePtr v = ValuePtr()) { return std::make_shared<Property>(id, n, v); }

private:
	IdType _id;
	String _name;
	ValuePtr _value;
};

struct LessProperty
{
	bool operator () (PropertyPtr x, PropertyPtr y) const
	{
		return x->name() < y->name();
	}
};

using PropertySet = set<PropertyPtr, LessProperty>;
using PropertyVector = vector<PropertyPtr>;
}