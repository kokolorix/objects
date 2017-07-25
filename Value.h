#pragma once

#include "Thing.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_serialize.hpp>
#include <boost/uuid/uuid_generators.hpp>

namespace
{
boost::uuids::random_generator GenerateId;
boost::uuids::nil_generator GenerateNullId;
boost::uuids::string_generator GenerateIdFromString;
}
using UuId = boost::uuids::uuid;

class Object;
using ObjectPtr = std::shared_ptr<Object>;

class Value;
using ValuePtr = std::shared_ptr<Value>;

template <typename T> class ValueImpl;

class Value : public Thing
{
 public:
	virtual ~Value() {}
	virtual operator String  () const { return toString(); }
	virtual operator int32_t  () const { throw NotImplementetException(__func__); }
	virtual operator int64_t  () const { throw NotImplementetException(__func__); }
	virtual operator uint64_t  () const { throw NotImplementetException(__func__); }
	virtual operator uint32_t  () const { throw NotImplementetException(__func__); }
	virtual operator float  () const { throw NotImplementetException(__func__); }
	virtual ThingPtr operator [] (size_t index) { throw NotImplementetException(__func__); }
	virtual ObjectPtr operator [] (String name) { throw NotImplementetException(__func__); }
	template<typename T>
	static std::shared_ptr<ValueImpl<T> > make(T v);
};

template <typename T>
class ValueImpl : public Value
{
 public:
	ValueImpl(T v) : _value(v) {}
	virtual operator T  () const { return _value; }

 private:
	T _value;
};

using StringValue = ValueImpl<String>;
using StringValuePtr = std::shared_ptr<StringValue>;
using Int32Value = ValueImpl<int32_t>;
using SInt32ValuePtr = std::shared_ptr<Int32Value>;
using FloatValue = ValueImpl<float>;
using floatValuePtr = std::shared_ptr<FloatValue>;
using UuIdValue = ValueImpl<UuId>;
using UuIdValuePtr = std::shared_ptr<UuIdValue>;

template<typename T>
inline std::shared_ptr<ValueImpl<T> > Value::make(T v)
{
	return std::make_shared<ValueImpl<T>>(v);
}
