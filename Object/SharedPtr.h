#pragma once

#include "Thing.h"

#include <memory>
#include <utility>
#include <vector>

namespace obj
{
using std::vector;
using std::pair;

class Value;
class Property;
class Object;

using IdType = uint32_t;
extern const IdType nullId;
}

namespace obj
{
struct ValuePtr : public shared_ptr<const Value>
{
	using Base = shared_ptr<const Value>;
	using Base::Base;
	ValuePtr() : Base() {}
	template<typename T>
	ValuePtr(T v);
	ValuePtr(const String::value_type * v);
	ValuePtr(std::initializer_list<ValuePtr> il);
	operator String() const;
	operator int32_t() const;
};
using ValuePtrVector = std::vector<ValuePtr>;
template <typename T> class ValueImpl;

struct PropertyPtr : public shared_ptr<Property>
{
	using Base = shared_ptr<Property>;
	using Base::Base;
	PropertyPtr() : Base() {}
	PropertyPtr(pair<String, ValuePtr> p);
};
using PropertyVector = vector<PropertyPtr>;

//struct ObjectPtr;
class Result;

struct ObjectPtr : public shared_ptr<Object>
{
	using Base = shared_ptr<Object>;
	using Base::Base;
	ObjectPtr() : Base() {}
	ObjectPtr(std::initializer_list<pair<String, ValuePtr>> r);
	Result operator [] (const String& name);
	Result operator = (ValuePtr value);
	ObjectPtr& operator () (const String& name);
	ObjectPtr& operator += (PropertyPtr p);
};
using ObjectPtrVector = std::vector<ObjectPtr>;

class Result
{
public:
	Result(const String& n, ObjectPtr& o) :_name(n), _object(o) {}
	Result(const String& n, ObjectPtr& o, ValuePtr r) :_name(n), _object(o), _result(r) {}
	Result(const Result& r) = default;
	Result(Result&& r) = default;

private:
	IdType _id;
	String _name;
	ObjectPtr _object;
	ValuePtr _result;

public:
	Result operator =(ValuePtr r);
	template<typename T>
	Result operator =(std::initializer_list<T> il);
	Result operator =(std::initializer_list<std::initializer_list<pair<String, ValuePtr>>> il);
	//Result operator =(std::initializer_list<pair<String, ValuePtr>> r);
	Result operator +=(ValuePtr r);
	ValuePtr& operator [] (size_t i);
	Result operator [] (const String& name);
	const Value& operator * () const { return *_result; }
	ValuePtr operator -> () { return _result; }
	operator ValuePtr () { return _result; }
	//operator String () { return _name; }
	template<class RT>
	operator shared_ptr<RT>() { return dynamic_pointer_cast<RT>(_result); }
};

template<typename T>
inline Result Result::operator=(std::initializer_list<T> il)
{
	VectorValuePtr v(VectorValue(il));
	return (*this) = v;
}
}