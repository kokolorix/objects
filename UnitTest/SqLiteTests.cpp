
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


obj::TestResult obj::Test::SqLiteTests::runTest()
{
	ObjectPtr obj1 = js::readFile("../.vscode/launch.json");
	db::writeObject("../objects.db3", obj1);

	ObjectPtr obj2 = db::readObject("../objects.db3", "");

	assert(obj1 == obj2);

	return TestResult::Successful;
}
