#pragma once

#include "Thing.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_serialize.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/lexical_cast.hpp>

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

	virtual operator String() const { return toString(); }
	virtual operator int32_t() const { throw NotImplementetException(__func__); }
	virtual operator int64_t() const { throw NotImplementetException(__func__); }
	virtual operator uint64_t() const { throw NotImplementetException(__func__); }
	virtual operator uint32_t() const { throw NotImplementetException(__func__); }
	virtual operator float() const { throw NotImplementetException(__func__); }
	virtual operator ObjectPtr() const { throw NotImplementetException(__func__); }

	virtual bool isInt32() const { return false; }
	virtual bool isInt64() const { return false; }
	virtual bool isUint64() const { return false; }
	virtual bool isUint32() const { return false; }
	virtual bool isFloat() const { return false; }
	virtual bool isObject() const { return false; }

	virtual ValuePtr operator [] (size_t index) { throw NotImplementetException(__func__); }
	virtual ValuePtr operator [] (String name) { throw NotImplementetException(__func__); }

	template<typename T>
	static std::shared_ptr<ValueImpl<T> > make(T v);
};

template <typename T>
class ValueImpl : public Value
{
 public:
	ValueImpl() : _value() {}
	ValueImpl(T v) : _value(v) {}
	//virtual operator T  () const { return _value; }

	//template<typename ConvT>
	//ConvT convert() const { return boost::lexical_cast<ConvT>(_value); }

	virtual operator int32_t() const;// { return convert<int32_t>(_value); }
	//virtual operator int64_t() const { return convert<int64_t>(_value); }
	//virtual operator uint64_t() const { return convert<uint64_t>(_value); }
	//virtual operator uint32_t() const { return convert<uint32_t>(_value); }
	//virtual operator float() const { return convert<float>(_value); }
	//virtual operator double() const { return convert<double>(_value); }
	//virtual operator ObjectPtr() const;// { return convert<ObjectPtr>(_value); }
	static std::shared_ptr<ValueImpl<T>> make(T v)
	{
		return std::make_shared<ValueImpl<T>>(v);
	}
private:
	T _value;
};
template<typename T>
inline ValueImpl<T>::operator int32_t () const 
{ 
	return boost::lexical_cast<int32_t>(_value); 
}

template<typename T>
inline std::shared_ptr<ValueImpl<T>> Value::make(T v)
{
	return std::make_shared<ValueImpl<T>>(v);
}

using StringValue = ValueImpl<String>;
using StringValuePtr = std::shared_ptr<StringValue>;
//template<> StringValue::operator int32_t() const { return boost::lexical_cast<int32_t>(_value); }

using BooleanValue = ValueImpl<bool>;
using BooleanValuePtr = std::shared_ptr<BooleanValue>;
//template<> Int32Value::operator int32_t() const { return boost::lexical_cast<int32_t>(_value); }

using Int32Value = ValueImpl<int32_t>;
using Int32ValuePtr = std::shared_ptr<Int32Value>;
//template<> Int32Value::operator int32_t() const { return boost::lexical_cast<int32_t>(_value); }

using FloatValue = ValueImpl<float>;
using floatValuePtr = std::shared_ptr<FloatValue>;
//template<> FloatValue::operator int32_t() const { return boost::lexical_cast<int32_t>(_value); }

using UuIdValue = ValueImpl<UuId>;
using UuIdValuePtr = std::shared_ptr<UuIdValue>;
template<> UuIdValue::operator int32_t() const { throw ImpossibleCastException(__func__); }

using ObjectValue = ValueImpl<ObjectPtr>;
using ObjectValuePtr = std::shared_ptr<ObjectPtr>;
template<> ObjectValue::operator int32_t() const { throw ImpossibleCastException(__func__); }
