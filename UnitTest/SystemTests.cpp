
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
	PropertyPtr Pi = Property::make("Pi", FloatValue::make(3.14f));
	PropertyPtr Id = Property::make("Id", UuIdValue::make(generateId()));
	ObjectPtr obj = Object::make(PropertySet{ Name, A2E, Pi });

	obj = js::readFile("../.vscode/tasks.json");

	Json j1 =
	{
		{ "pi", 3.141 },
		{ "happy", true },
		{ "name", "Niels" },
		{ "nothing", nullptr },
		{ "answer",{
			{ "everything", 42 }
		} },
		{ "list",{ 1, 0, 2 } },
		{ "object",{
			{ "currency", "USD" },
			{ "value", 42.99 }
		} }
	};
	String s1 = j1.dump(4);
	auto j3 = Json::parse(s1);
	String s3 = j3.dump(4);

	obj = js::readJson(j1);

	Json j2 = js::writeJson(obj);
	String s2 = j2.dump(4);

	//assert(j1 == j2);

	//int32_t id = *Id->value();

	//DataStorePtr pDataStore = make_shared<DataStore>();
	//pDataStore->Name = "MyTestStore";
	//pDataStore->Id = GenerateIdFromString("1089B36E-EB63-45AC-B8C2-D38EE9A60DDF");

	//ElementPtr pRootElement = make_shared<Element>("Root", PropertySet{
	//	make_shared<Property>("StoreId", make_shared<UuidValue>(pDataStore->Id)),
	//	make_shared<Property>("StoreName", make_shared<StringValue>(pDataStore->Name))
	//});

	//ElementPtr pEnvironmentElement = make_shared<Element>("Environment", PropertySet{
	//	make_shared<Property>("Directory", make_shared<StringValue>("/User/Tata")),
	//	make_shared<Property>("StoreName", make_shared<StringValue>(pDataStore->Name))
	//});
	//pEnvironmentElement->ParentId = pRootElement->Id;

	//pDataStore->addElement(pRootElement);


	return TestResult::Successful;
}

