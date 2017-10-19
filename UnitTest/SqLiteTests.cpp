
#include "stdafx.h"

#include "SqLiteTests.h"
using namespace obj;

// #include "Data/DataStore.h"
#include "Value.h"
#include "SqLite.h"
#include "Json.h"

namespace
{
	bool test = Test::addTestCase(make_shared<Test::SqLiteTests>(), "SqLiteTests");
}

namespace sqlT
{
void simpleTest();
}

obj::TestResult obj::Test::SqLiteTests::runTest()
{
	sqlT::simpleTest();

	return TestResult::Successful;
}

void sqlT::simpleTest()
{
	{
		Json j =	{
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
		//ObjectPtr obj1 = js::readJson(j); //obj1->id() = 13;
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

		//fs::remove("../test.db3");
		//IdType id1 = db::writeObject("../test.db3", obj1);
		//ObjectPtr obj2 = db::readObject("../test.db3", id1);
		DbHolder db(":memory:");
		IdType id1 = db.writeObject(obj1);
		ObjectPtr obj2 = db.readObject(id1);

		String str1 = js::writeJson(obj1).dump(4);
		String str2 = js::writeJson(obj2).dump(4);
		assert(str1 == str2);		
		assert(obj1 == obj2);
	}
}

