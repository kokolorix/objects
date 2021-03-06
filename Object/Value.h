#pragma once

#include "SharedPtr.h"

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
}

template<>
inline obj::String boost::lexical_cast(const bool& v) { return v ? obj::String("true") : obj::String("false"); }

template<>
inline bool boost::lexical_cast(const obj::String& str)
{ 
	if (str == "true")
		return true;
	else if(str == "false")
		return false;
	else
		throw obj::ImpossibleCastException(obj::Format("'%1%' is neither true nor false"));
}

template<>
inline obj::UuId boost::lexical_cast(const obj::String& str) { return obj::generateIdFromString(str); }

namespace obj
{
bool operator < (ObjectPtr x, ObjectPtr y);
bool operator == (ObjectPtr x, ObjectPtr y);

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
	static ValuePtr parse(const String& s);

	static ValuePtr make();
	static ValuePtr make(std::initializer_list<ValuePtr> v);

	template<typename T>
	static ValuePtr make(T v);
	template<typename T>
	static ValuePtr make(IdType id, T v);

protected:
	IdType _id;
};

class Unknown {};
template <typename T = Unknown>
class NothingValue : public Value
{
public:
	static shared_ptr<NothingValue<T>> make()
	{
		return make_shared<NothingValue<T>>();
	}
	virtual String toString() const  override { return String(); }
	virtual bool operator<(const Thing & other) const override
	{
		if (auto o = dynamic_cast<decltype(this)>(&other))
			return *this < *o;
		else
			return Thing::operator<(other);
	}
	bool operator < (const NothingValue& other) const
	{
		if (_id && other._id)
			return _id < other._id;
		else
			return false;
	}
};
using NothingValuePtr = shared_ptr<NothingValue<Unknown>>;

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
		if (_id && other._id)
			return std::tie(_id, _value) < std::tie(other._id, other._value);
		else
			return _value < other._value;
	}

public:
	const T& value() const { return _value; }
	T& value() { return _value; }

	static std::shared_ptr<ValueImpl<T>> make(T v)
	{
		return make_shared<ValueImpl<T>>(v);
	}

	virtual String toString() const
	{
		return lexical_cast<String>(_value);
	}
	
private:
	T _value;
};

template<typename T>
inline ValuePtr Value::make(T v){	return make_shared<ValueImpl<T>>(v);}
template<>
inline ValuePtr Value::make(const String::value_type* v){	return make_shared<ValueImpl<String>>(v);}
template<>
inline ValuePtr Value::make(Format v){	return make_shared<ValueImpl<String>>(v.str());}

template<typename T>
inline ValuePtr Value::make(IdType id, T v){	return make_shared<ValueImpl<T>>(id, v);}
template<>
inline ValuePtr Value::make(IdType id, const String::value_type* v){	return make_shared<ValueImpl<String>>(id, v);}
template<>
inline ValuePtr Value::make(IdType id, Format v){	return make_shared<ValueImpl<String>>(id, v.str());}

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
template<>
bool FloatValue::operator<(const FloatValue& other) const
{
	if (_id && other._id)
		return std::tie(_id, _value) < std::tie(other._id, other._value);
	else
		return _value < other._value;
}
using UuIdValue = const ValueImpl<UuId>;
using UuIdValuePtr = std::shared_ptr<UuIdValue>;
template<>
String UuIdValue::toString() const
{
	using boost::uuids::to_string;
	return to_string(_value);
}

using ObjectValue = const ValueImpl<ObjectPtr>;
using ObjectValuePtr = std::shared_ptr<ObjectValue>;
template<>
String ObjectValue::toString() const;

using VectorValue = const ValueImpl<ValuePtrVector>;
using VectorValuePtr = shared_ptr<VectorValue>;
template<>
String VectorValue::toString() const;

inline bool operator < (ValuePtr x, ValuePtr y)
{
	bool result = (x.get() && y.get()) ? (*x < *y) : x.get() < y.get();
	return result;
}
inline bool operator == (ValuePtr x, ValuePtr y)
{
	bool result = x.get() == y.get() || (x.get() && y.get() && !(*x < *y) && !(*y < *x));
	return result;
}
template<typename T>
inline bool operator == (const Value& x, T y)
{
	if(auto xv = dynamic_cast<const ValueImpl<T>*>(&x))
		return xv->value() == y;
	else 
		return false;
}
template<>
inline bool operator == (const Value& x, const char* y)
{
	return x == String(y);
}

template< typename T >
inline ValuePtr::ValuePtr(T v) : Base(Value::make(v))
{
}
inline ValuePtr::ValuePtr(const String::value_type* v) : Base(Value::make(v))
{
}

}
