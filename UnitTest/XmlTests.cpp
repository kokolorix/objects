
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
"	{"
"		\"answer\": {"
"		\"everything\": 42"
"	},"
"		\"happy\" : true,"
"			\"list\" : ["
"				1,"
"					0,"
"					2,"
"					{ \"einWert\":35 },"
"					[9,8,7]"
"			],"
"			\"name\": \"Niels\","
"				\"nothing\" : null,"
"				\"object\" : {"
"				\"currency\": \"USD\","
"					\"value\" : 42.9900016784668"
"			},"
"				\"pi\" : 3.14100003242493"
"	}"
));
	//obj1["list"]->push_back(nullptr);
	//String str1 = xml::writeString(obj1);
	xml::writeFile("./TestResult.xml", obj1);
	//ObjectPtr obj2 = xml::readString(str1);
	//String str2 = xml::writeString(obj2);
	//assert(str1 == str2);		
	//assert(obj1 == obj2);
}

