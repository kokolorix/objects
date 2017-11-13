
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

namespace sysT
{
	void simpleTest();
	void htmlTest();
	void bigTest();
}

obj::TestResult obj::Test::SystemTests::runTest()
{
	using namespace sysT;

	simpleTest();
	htmlTest();
	bigTest();
	
	return TestResult::Successful;
}

void sysT::bigTest()
{
	ObjectPtr root = Object::make();

	for (rsize_t oi = 0; oi < 1000; ++oi)
	{
		ObjectPtr obj = Object::make();
		String name = str(Format("Object%1%") % oi);
		obj[name] = name;

		for (size_t pi = 0; pi < 200; pi++)
		{	
			String name = str(Format("Propert%1%") % pi);
			PropertyPtr prp = Property::make(name);
			int type = pi % 5;
			if (type == 0)
			{
				prp->value() = Value::make(Format("Stringvalue %1%") % pi);
			}
			else if (type == 1)
			{

			}
			else if (type == 2)
			{

			}
			else if (type == 3)
			{

			}
			else if (type == 4)
			{

			}
		}
	}

}
void sysT::htmlTest()
{
	ObjectPtr html = Object::make();
	html["lang"] = "en";
	html("head")["meta"] = {
		{ {"charset", "utf-8"} },
		{ { "name", "viewport" }, {"content", "width=device-width, initial-scale=1.0"} }
	};
	html("head")("title") = "SystemTest";
	html("head")("style") = "#itm { margin-left:25px; }";

	html("body")["details"] = "SystemTest";

	//["charset"] = "utf-8";
	//(html("head")("meta")["name"] = "viewport") ["content"] = "width=device-width, initial-scale=1.0";
}
void sysT::simpleTest()
{
	ValuePtr a2e = Value::make(42);
	PropertyPtr Name = Property::make("Name", StringValue::make("TheFirstObject"));
	PropertyPtr A2E = Property::make("AnswerToEverything", a2e);
	PropertyPtr Pi = Property::make("Pi", 3.14);
	PropertyPtr Id = Property::make("Id", generateId());
	ObjectPtr obj = Object::make(PropertySet{ Name, A2E, Pi });
	obj["Nr"] = 13;
	obj += Property::make("List", { 3.33, 624, generateId(), "a string" });
	obj["List"] += 19;
	assert(*obj["Name"] == "TheFirstObject");
	assert(*obj["Pi"] == 3.14);
	assert(*obj["Nr"] == 13);
	assert(*obj["List"][0] == 3.33);
	assert(*obj["List"][1] == 624);
	assert(*obj["List"][3] == "a string");
	assert(*obj["List"][4] == 19);
	assert(obj["Id"]->id() == 0);
}

