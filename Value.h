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

class Value;
struct ValuePtr : public shared_ptr<const Value>
{
	using Base = shared_ptr<const Value>;
	using Base::Base;
	//ValuePtr operator [] (String name);
	operator String() const;
	operator int32_t() const;
};
using ValuePtrVector = std::vector<ValuePtr>;
template <typename T> class ValueImpl;


class Object;
struct ObjectPtr;
struct PropertyResult
{
	String _name;
	ObjectPtr _object;
	ValuePtr _value;

	PropertyResult& operator =(ValuePtr value);
	operator ValuePtr () { return _value; }
};

struct ObjectPtr : public shared_ptr<Object>
{
	using Base = shared_ptr<Object>;
	using Base::Base;
	ValuePtr operator [] (String name);
};

using ObjectPtrVector = std::vector<ObjectPtr>;

class Value : public Thing
{
	friend class std::_Ref_count_obj<Value>;
protected:
	virtual ~Value() {}
public:
	operator String () const { return toString(); }
	template<typename T>
	static std::shared_ptr<ValueImpl<T> > make(T v);
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
	virtual String toString() { return String(); }
};
using NothingValuePtr = std::shared_ptr<NothingValue<Unknown>>;

template <typename T>
class ValueImpl : public Value
{
	friend class std::_Ref_count_obj<ValueImpl>;
protected:
	ValueImpl() : _value() {}
	ValueImpl(T v) : _value(v) {}
	virtual ~ValueImpl() {}
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
inline std::shared_ptr<ValueImpl<T>> Value::make(T v)
{
	return std::make_shared<ValueImpl<T>>(v);
}

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
