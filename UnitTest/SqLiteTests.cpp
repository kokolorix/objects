
#include "stdafx.h"

#include "SqLiteTests.h"
using namespace obj;

// #include "Data/DataStore.h"
#include "Value.h"
#include "Sqlite.h"
#include "Json.h"

namespace
{
	bool test = Test::addTestCase(make_shared<Test::SqLiteTests>(), "SqLiteTests");
}

bool operator < (ObjectPtr x, ObjectPtr y)
{
	return x.get() < y.get();
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
		ObjectPtr obj1 = js::readJson(j); //obj1->id() = 13;
		//IdType id = db::writeObject("../objects.db3", obj1);
		ObjectPtr obj2 = db::readObject("../objects.db3", 13);
		String str1 = js::writeJson(obj1).dump(4);
		String str2 = js::writeJson(obj2).dump(4);
		assert(str1 == str2);		
		assert(obj1 == obj2);
	}
}

