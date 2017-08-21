#include "Value.h"
#include <boost/preprocessor.hpp>
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
	f[BOOST_STRINGIZE(UuIdValue)] = std::bind(&UuIdValue::make, GenerateNullId());
	f[BOOST_STRINGIZE(ObjectValue)] = std::bind(&ObjectValue::make, ObjectPtr());
	f[BOOST_STRINGIZE(VectorValue)] = std::bind(&VectorValue::make, std::vector<ValuePtr>());
	return true;
}

obj::ValuePtr::operator int32_t() const
{
	if (auto p = std::dynamic_pointer_cast<StringValue>(*this))
		return boost::lexical_cast<int32_t>(p->value());
	throw NotImplementetException(__func__);
}
