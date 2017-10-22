
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
	PropertyPtr Pi = Property::make("Pi", 3.14);
	PropertyPtr Id = Property::make("Id",generateId());
	ObjectPtr obj = Object::make(PropertySet{ Name, A2E, Pi });
	obj["Nr"] = 13;
	obj += Property::make("List", {3.33, 624, generateId(), "a string"});
	obj["List"] += 19;
	assert(*obj["Name"] == "TheFirstObject");
	assert(*obj["Pi"] == 3.14);
	assert(*obj["Nr"] == 13);
	assert(*obj["List"][0] == 3.33);
	assert(*obj["List"][1] == 624);
	assert(*obj["List"][3] == "a string");
	assert(*obj["List"][4] == 19);
	assert(obj["Id"]->id() == 0);

	return TestResult::Successful;
}

