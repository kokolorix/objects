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

namespace
{
boost::uuids::random_generator GenerateId;
boost::uuids::nil_generator GenerateNullId;
//boost::uuids::string_generator GenerateIdFromString;
}
namespace obj
{
using UuId = boost::uuids::uuid;

class Object;
using ObjectPtr = shared_ptr<Object>;
using ObjectPtrVector = std::vector<ObjectPtr>;

class Value;
using ValuePtr = shared_ptr<Value>;
using ValuePtrVector = std::vector<ValuePtr>;
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

	//virtual bool isInt32() const { return false; }
	//virtual bool isInt64() const { return false; }
	//virtual bool isUint64() const { return false; }
	//virtual bool isUint32() const { return false; }
	//virtual bool isFloat() const { return false; }
	//virtual bool isObject() const { return false; }

	//virtual ValuePtr operator [] (size_t index) { throw NotImplementetException(__func__); }
	//virtual ValuePtr operator [] (String name) { throw NotImplementetException(__func__); }

	template<typename T>
	static std::shared_ptr<ValueImpl<T> > make(T v);
};

template <typename T>
class ValueImpl : public Value
{
public:
	ValueImpl() : _value() {}
	ValueImpl(T v) : _value(v) {}
	const T& value() const { return _value; }
	T& value() { return _value; }
	//virtual operator T  () const { return _value; }

	template<typename ConvT>
	ConvT convert() const;// { return boost::lexical_cast<ConvT>(_value); }

	virtual operator int32_t() const { return convert<int32_t>(); }
	virtual operator int64_t() const { return convert<int64_t>(); }
	virtual operator uint64_t() const { return convert<uint64_t>(); }
	virtual operator uint32_t() const { return convert<uint32_t>(); }
	virtual operator float() const { return convert<float>(); }
	//virtual operator double() const { return convert<double>(); }
	//virtual operator ObjectPtr() const { return convert<ObjectPtr>(); }

	static std::shared_ptr<ValueImpl<T>> make(T v)
	{
		return std::make_shared<ValueImpl<T>>(v);
	}
private:
	T _value;
};

template<typename T>
inline std::shared_ptr<ValueImpl<T>> Value::make(T v)
{
	return std::make_shared<ValueImpl<T>>(v);
}

using StringValue = ValueImpl<String>;
using StringValuePtr = std::shared_ptr<StringValue>;

using BooleanValue = ValueImpl<bool>;
using BooleanValuePtr = std::shared_ptr<BooleanValue>;

using Int32Value = ValueImpl<int32_t>;
using Int32ValuePtr = std::shared_ptr<Int32Value>;

using UInt32Value = ValueImpl<uint32_t>;
using UInt32ValuePtr = std::shared_ptr<UInt32Value>;

using FloatValue = ValueImpl<double>;
using floatValuePtr = std::shared_ptr<FloatValue>;

using UuIdValue = ValueImpl<UuId>;
using UuIdValuePtr = std::shared_ptr<UuIdValue>;

using ObjectValue = ValueImpl<ObjectPtr>;
using ObjectValuePtr = std::shared_ptr<ObjectPtr>;

using VectorValue = ValueImpl<std::vector<ValuePtr> >;
using VectorValuePtr = std::shared_ptr<VectorValue>;
}

namespace boost
{
using namespace obj;
template<> inline int32_t lexical_cast(const UuIdValue&) { throw ImpossibleCastException(__func__); }
template<> inline int32_t lexical_cast(const ObjectPtr&) { throw ImpossibleCastException(__func__); }
template<> inline int32_t lexical_cast(const std::vector<ValuePtr>&) { throw ImpossibleCastException(__func__); }
template<> inline uint32_t lexical_cast(const UuIdValue&) { throw ImpossibleCastException(__func__); }
template<> inline uint32_t lexical_cast(const ObjectPtr&) { throw ImpossibleCastException(__func__); }
template<> inline uint32_t lexical_cast(const std::vector<ValuePtr>&) { throw ImpossibleCastException(__func__); }
template<> inline int64_t lexical_cast(const UuIdValue&) { throw ImpossibleCastException(__func__); }
template<> inline int64_t lexical_cast(const ObjectPtr&) { throw ImpossibleCastException(__func__); }
template<> inline int64_t lexical_cast(const std::vector<ValuePtr>&) { throw ImpossibleCastException(__func__); }
template<> inline uint64_t lexical_cast(const UuIdValue&) { throw ImpossibleCastException(__func__); }
template<> inline uint64_t lexical_cast(const ObjectPtr&) { throw ImpossibleCastException(__func__); }
template<> inline uint64_t lexical_cast(const std::vector<ValuePtr>&) { throw ImpossibleCastException(__func__); }
template<> inline float lexical_cast(const UuIdValue&) { throw ImpossibleCastException(__func__); }
template<> inline float lexical_cast(const ObjectPtr&) { throw ImpossibleCastException(__func__); }
template<> inline float lexical_cast(const std::vector<ValuePtr>&) { throw ImpossibleCastException(__func__); }

//template<> inline ObjectPtr lexical_cast(const UuIdValue&) { throw ImpossibleCastException(__func__); }
//template<> inline ObjectPtr lexical_cast(ObjectPtr) { throw ImpossibleCastException(__func__); }
//template<> inline ObjectPtr lexical_cast(const std::vector<ValuePtr>&) { throw ImpossibleCastException(__func__); }
//template<> inline ObjectPtr lexical_cast(const int32_t&) { throw ImpossibleCastException(__func__); }
//template<> inline ObjectPtr lexical_cast(const uint32_t&) { throw ImpossibleCastException(__func__); }
//template<> inline ObjectPtr lexical_cast(const int64_t&) { throw ImpossibleCastException(__func__); }
//template<> inline ObjectPtr lexical_cast(const uint64_t&) { throw ImpossibleCastException(__func__); }
}

namespace obj
{
template<typename T>
template<typename ConvT>
inline ConvT ValueImpl<T>::convert() const
{
	return boost::lexical_cast<ConvT>(_value);
}
template<>
template<>
inline ObjectPtr ValueImpl<UuIdValue>::convert() const
{
	throw ImpossibleCastException(__func__);
}
//template<typename T>
//inline ValueImpl<T>::operator int32_t () const
//{
//	return boost::lexical_cast<int32_t>(_value);
//}
}