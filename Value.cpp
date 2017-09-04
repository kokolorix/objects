#include "stdafx.h"
#include "Value.h"
#include <boost/preprocessor.hpp>
#include <boost/algorithm/string.hpp>
using namespace obj;
bool registerCreators();
bool intialized = registerCreators();
bool registerCreators()
{
	ThingFactory& f = ThingFactory::instance();


	f[BOOST_STRINGIZE(StringValue)] = std::bind(&StringValue::make, String());
	f[BOOST_STRINGIZE(BooleanValue)] = std::bind(&BooleanValue::make, false);
	f[BOOST_STRINGIZE(Int32Value)] = std::bind(&Int32Value::make, 0);
	f[BOOST_STRINGIZE(UInt32Value)] = std::bind(&UInt32Value::make, 0);
	f[BOOST_STRINGIZE(FloatValue)] = std::bind(&FloatValue::make, 0.0);
	f[BOOST_STRINGIZE(UuIdValue)] = std::bind(&UuIdValue::make, generateNullId());
	f[BOOST_STRINGIZE(ObjectValue)] = std::bind(&ObjectValue::make, ObjectPtr());
	f[BOOST_STRINGIZE(VectorValue)] = std::bind(&VectorValue::make, std::vector<ValuePtr>());
	return true;
}

boost::uuids::random_generator obj::generateId;
boost::uuids::nil_generator obj::generateNullId;
boost::uuids::string_generator obj::generateIdFromString;

template<>
String VectorValue::toString() const
{
	std::ostringstream os;
	bool first = true;
	for (ValuePtr v : _value)
	{
		if (first)
			first = false;
		else
			os << ",";
		os << "\"" << v->toString() << "\"";
	}
	return os.str();
}


obj::ValuePtr::operator String() const
{
	return (*this)->toString();
}

obj::ValuePtr::operator int32_t() const
{
	if (auto p = dynamic_pointer_cast<const Int32Value>(*this))
		return p->value();
	else if (auto p = dynamic_pointer_cast<const StringValue>(*this))
		return boost::lexical_cast<int32_t>(p->value());
	throw ImpossibleCastException(__func__);
}
