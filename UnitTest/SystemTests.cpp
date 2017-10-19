
#include "stdafx.h"

#include "SystemTests.h"
using namespace obj;

// #include "Data/DataStore.h"
#include "Value.h"
#include "Json.h"

namespace
{
	bool test = Test::addTestCase(make_shared<Test::SystemTests>(), "SystemTests");
}

obj::TestResult obj::Test::SystemTests::runTest()
{
	ValuePtr a2e = Value::make(42);
	PropertyPtr Name = Property::make("Name", StringValue::make("TheFirstObject"));
	PropertyPtr A2E = Property::make("AnswerToEverything", a2e);
	PropertyPtr Pi = Property::make("Pi", FloatValue::make(3.14));
	PropertyPtr Id = Property::make("Id", UuIdValue::make(generateId()));
	ObjectPtr obj = Object::make(PropertySet{ Name, A2E, Pi });

	return TestResult::Successful;
}

