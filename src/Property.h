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
//class Property;
//using PropertyPtr = shared_ptr<Property>;
//
class Property : public Thing
{
public:
	Property(String n, ValuePtr v = ValuePtr()) : _id(nullId), _name(n), _value(v) {}
	Property(IdType id, String n, ValuePtr v = ValuePtr()) : _id(id), _name(n), _value(v) {}
	Property() : _id(nullId) {}
	virtual ~Property() {}
	IdType id() const { return _id; }
	IdType& id() { return _id; }
	operator String  () const { return toString(); }
	virtual bool operator<(const Thing & other) const override
	{
		if (auto o = dynamic_cast<decltype(this)>(&other))
			return *this < *o;
		else
			return Thing::operator<(other);
	}
	bool operator < (const Property& other) const;
	const String& name() const { return _name; }
	String& name() { return _name; }
	ValuePtr value() const { return _value; }
	ValuePtr& value() { return _value; }
	static PropertyPtr make(String n, ValuePtr v = ValuePtr()) { return make_shared<Property>(n, v); }
	static PropertyPtr make(IdType id, String n, ValuePtr v = ValuePtr()) { return make_shared<Property>(id, n, v); }
	template<typename T>
	inline static PropertyPtr make(String n, shared_ptr<ValueImpl<T>> v) 
	{
		return make_shared<Property>(n, v); 
	}
	template<typename T>
	inline static PropertyPtr make(String n, T v)
	{
		return make_shared<Property>(n, Value::make(v)); 
	}
	inline static PropertyPtr make(String n, std::initializer_list<ValuePtr> v)
	{
		return make_shared<Property>(n, Value::make(v)); 
	}

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
inline bool operator < (PropertyPtr x, PropertyPtr y)
{
	bool result = (x.get() && y.get()) ? (*x < *y) : x.get() < y.get();
	return result;
}
inline bool operator == (PropertyPtr x, PropertyPtr y)
{
	bool result = x.get() == y.get() || (x.get() && y.get() && !(*x < *y) && !(*y < *x));
	return result;
}


}