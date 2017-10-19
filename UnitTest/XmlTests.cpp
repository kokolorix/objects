
#include "stdafx.h"

#include "XmlTests.h"
using namespace obj;

// #include "Data/DataStore.h"
#include "Value.h"
#include "SqLite.h"
#include "Json.h"
#include "Xml.h"

namespace
{
	bool test = Test::addTestCase(make_shared<Test::XmlTests>(), "XmlTests");
}

namespace xmlT
{
void simpleTest();
}

obj::TestResult obj::Test::XmlTests::runTest()
{
	xmlT::simpleTest();
	return TestResult::Successful;
}

void xmlT::simpleTest()
{
	//ObjectPtr obj1 = js::readJson(Json {
	//	{ "pi", 3.141 },
	//	{ "happy", true },
	//	{ "name", "Niels" },
	//	{ "nothing", nullptr },
	//	{ "answer",{
	//		{ "everything", 42 }
	//	} },
	//	{ "list",{ 1, 0, 2 } },
	//	{ "object",{
	//		{ "currency", "USD" },
	//		{ "value", 42.99 }
	//	} }
	//});
	ObjectPtr obj1 = js::readJson(Json::parse(
		R"js(	{)js"
		R"js(		"answer": {)js"
		R"js(		"everything": 42)js"
		R"js(	},)js"
		R"js(		"happy" : true,)js"
		R"js(			"list" : [)js"
		R"js(				1,)js"
		R"js(					0,)js"
		R"js(					2,)js"
		R"js(					{ "einWert":35 },)js"
		R"js(					[9,8,7])js"
		R"js(			],)js"
		R"js(			"name": "Niels",)js"
		R"js(				"nothing" : null,)js"
		R"js(				"object" : {)js"
		R"js(				"currency": "USD",)js"
		R"js(					"value" : 42.99)js"
		R"js(			},)js"
		R"js(				"pi" : 3.141)js"
		R"js(	})js"
	));
	xml::writeFile("./obj1.xml", obj1);
	ObjectPtr obj2 = xml::readFile("./obj1.xml");
	xml::writeFile("./obj2.xml", obj2);
	String str1 = xml::writeString(obj1);
	String str2 = xml::writeString(obj2);
	assert(str1 == str2);		
	assert(obj1 == obj2);
}

