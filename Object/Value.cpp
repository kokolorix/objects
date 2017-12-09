#include "stdafx.h"
//#define _CRT_SECURE_NO_WARNINGS
#include "Value.h"
#include "Property.h"
#include "Object.h"
#include <boost/preprocessor.hpp>
#include <boost/algorithm/string.hpp>
#include <regex>

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
const IdType obj::nullId = 0;

template<>
String VectorValue::toString() const
{
	std::ostringstream os;
	for (size_t i = 0; i < _value.size(); ++i)
	{
		if(i)
			os << ",";
		os << "\"" << _value[i]->toString() << "\"";
	}
	return os.str();
}

obj::ValuePtr::ValuePtr(std::initializer_list<ValuePtr> r)
	: Base(Value::make(r))
{
}

obj::ValuePtr::operator String() const
{
	return (*this) ? (*this)->toString() : String();
}

obj::ValuePtr::operator int32_t() const
{
	if (auto p = dynamic_pointer_cast<const Int32Value>(*this))
		return p->value();
	else if (auto p = dynamic_pointer_cast<const StringValue>(*this))
		return boost::lexical_cast<int32_t>(p->value());
	throw ImpossibleCastException(__func__);
}

ValuePtr obj::Value::parse(const String & s)
{
	if (s == "true")
		return Value::make(true);
	if (s == "false")
		return Value::make(false);

	char* e;
	int32_t i32 = std::strtol(s.c_str(), &e, 0);
	if (e != s.c_str() && *e == '\0') 
		return Value::make(i32);

	double d = strtod(s.c_str(), &e);
	if (e != s.c_str() && *e == '\0') 
		return Value::make(d);

	static const std::regex rx("[a-f0-9]{8}-[a-f0-9]{4}-4[a-f0-9]{3}-[89aAbB][a-f0-9]{3}-[a-f0-9]{12}");
	if(std::regex_match(s, rx))
		return Value::make(lexical_cast<UuId>(s));

	return Value::make(s);

	/*
	try
	{
		return Value::make(std::stoi(s));
	}
	catch(std::invalid_argument&){}
	catch(std::out_of_range&){}
	try
	{
		return Value::make(std::stod(s));
	}
	catch(std::invalid_argument&){}
	catch(std::out_of_range&){}
	try
	{
		return Value::make(lexical_cast<UuId>(s));
	}
	catch(std::exception&){}

	if (s == "true")
		return Value::make(true);
	if (s == "false")
		return Value::make(false);

	return Value::make(s);
	*/
}

ValuePtr obj::Value::make()
{
	return make_shared<NothingValue<Unknown>>();
}

ValuePtr obj::Value::make(std::initializer_list<ValuePtr> v)
{
	return make_shared<ValueImpl<ValuePtrVector>>(ValuePtrVector(v));
}

