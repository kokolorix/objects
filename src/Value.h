#pragma once

#include "Thing.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_serialize.hpp>
#ifdef __MINGW32__
#pragma GCC diagnostic ignored "-Wconversion-null"
#endif
#include <boost/uuid/uuid_generators.hpp>
#ifdef __MINGW32__
#pragma GCC diagnostic pop
#endif
#include <boost/lexical_cast.hpp>


namespace obj
{
using UuId = boost::uuids::uuid;
extern boost::uuids::random_generator generateId;
extern boost::uuids::nil_generator generateNullId;
extern boost::uuids::string_generator generateIdFromString;

using boost::lexical_cast;

using IdType = uint32_t;
extern const IdType nullId;

class Value;
}

template<>
inline obj::UuId boost::lexical_cast(const obj::String& str) { return obj::generateIdFromString(str); }

namespace obj
{
struct ValuePtr : public shared_ptr<const Value>
{
	using Base = shared_ptr<const Value>;
	using Base::Base;
	//ValuePtr operator [] (String name);
	ValuePtr() : Base() {}
	template<typename T>	ValuePtr(const T& v);
	operator String() const;
	operator int32_t() const;
};
using ValuePtrVector = std::vector<ValuePtr>;
template <typename T> class ValueImpl;

template<class ContainerT>
class Result
{
public:
	Result(const String& n, ContainerT& c ):_name(n), _container(c) {}
	Result(const String& n, ContainerT& c, ValuePtr r ):_name(n), _container(c), _result(r) {}
	Result(const Result& r) = default;

private:
	IdType _id;
	String _name;
	ContainerT _container;
	ValuePtr _result;
	
public:
	Result& operator =(ValuePtr r) { return *this; }
	operator ValuePtr () { return _result; }
	operator String () { return _result; }
	template<class ResT>
	operator shared_ptr<ResT>() { return dynamic_pointer_cast<ResT>(_result); }
};

class Object;
struct ObjectPtr : public shared_ptr<Object>
{
	using Base = shared_ptr<Object>;
	using Base::Base;
	Result<ObjectPtr> operator [] (String name);
};

using ObjectPtrVector = std::vector<ObjectPtr>;

class Value : public Thing
{
protected:
	Value() : _id(nullId){}
	Value(IdType id) : _id(id){}
	virtual ~Value() {}
public:
	IdType id() const { return _id; }
	IdType& id() { return _id; }
	operator String () const { return toString(); }
	template<typename T>
	static std::shared_ptr<ValueImpl<T> > make(T v);
	template<typename T>
	static std::shared_ptr<ValueImpl<T> > make(IdType id, T v);
private:
	IdType _id;
};

class Unknown {};
template <typename T = Unknown>
class NothingValue : public Value
{
public:
	static std::shared_ptr<NothingValue<T>> make()
	{
		return std::make_shared<NothingValue<T>>();
	}
	virtual String toString() const  override { return String(); }
};
using NothingValuePtr = std::shared_ptr<NothingValue<Unknown>>;

template <typename T>
class ValueImpl : public Value
{
public:
	ValueImpl() : _value() {}
	ValueImpl(T v) : _value(v) {}
	ValueImpl(IdType id, T v) : Value(id), _value(v) {}
	virtual ~ValueImpl() {}
	virtual bool operator<(const Thing & other) const override
	{
		if (auto o = dynamic_cast<decltype(this)>(&other))
			return *this < *o;
		else
			return Thing::operator<(other);
	}
	bool operator < (const ValueImpl& other) const
	{
		return _value < other._value;
	}

public:
	const T& value() const { return _value; }
	T& value() { return _value; }

	static std::shared_ptr<ValueImpl<T>> make(T v)
	{
		return std::make_shared<ValueImpl<T>>(v);
	}

	virtual String toString() const
	{
		return boost::lexical_cast<String>(_value);
	}
	
private:
	T _value;
};

template<typename T>
inline std::shared_ptr<ValueImpl<T>> Value::make(T v){	return std::make_shared<ValueImpl<T>>(v);}

template<typename T>
inline std::shared_ptr<ValueImpl<T>> Value::make(IdType id, T v){	return std::make_shared<ValueImpl<T>>(id, v);}

using StringValue = const ValueImpl<String>;
using StringValuePtr = std::shared_ptr<StringValue>;

using BooleanValue = const ValueImpl<bool>;
using BooleanValuePtr = std::shared_ptr<BooleanValue>;

using Int32Value = const ValueImpl<int32_t>;
using Int32ValuePtr = std::shared_ptr<Int32Value>;

using UInt32Value = const ValueImpl<uint32_t>;
using UInt32ValuePtr = std::shared_ptr<UInt32Value>;

using FloatValue = const ValueImpl<double>;
using floatValuePtr = std::shared_ptr<FloatValue>;

using UuIdValue = const ValueImpl<UuId>;
using UuIdValuePtr = std::shared_ptr<UuIdValue>;
template<>
String UuIdValue::toString() const
{
	using boost::uuids::to_string;
	return to_string(_value);
}

using ObjectValue = const ValueImpl<ObjectPtr>;
using ObjectValuePtr = std::shared_ptr<ObjectPtr>;
template<>
String ObjectValue::toString() const;

using VectorValue = const ValueImpl<ValuePtrVector>;
using VectorValuePtr = std::shared_ptr<VectorValue>;
template<>
String VectorValue::toString() const;

}
template< typename T >
inline obj::ValuePtr::ValuePtr(const T & v) : Base(Value::make(v))
{
}