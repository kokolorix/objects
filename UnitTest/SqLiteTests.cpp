
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
	//ObjectPtr obj1 = js::readFile("../.vscode/tasks.json");

	//ObjectPtr obj2 = db::readObject("../objects.db3", "");

	//assert(obj1 == obj2);
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
		ObjectPtr obj1 = js::readJson(j);
		IdType id = db::writeObject("../objects.db3", obj1);
		ObjectPtr obj2 = db::readObject("../objects.db3", id);
		assert(obj1 == obj2);
	}
}

