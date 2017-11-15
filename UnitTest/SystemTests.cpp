
#include "stdafx.h"
#include <cmath>
#include "windows.h"
#include "psapi.h"

#include "SystemTests.h"
using namespace obj;

// #include "Data/DataStore.h"
#include "Value.h"
#include "Json.h"
#include "SqLite.h"

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
	root["Objects"] = VectorValue::make(ValuePtrVector());

	for (rsize_t oi = 0; oi < 50001; ++oi)
	{
		ObjectPtr obj = Object::make();
		String name = str(Format("Object%1%") % oi);
		obj["Name"] = name;
		//root[name] = obj;
		root["Objects"] += obj;

		for (size_t pi = 0; pi < 100; pi++)
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
				prp->value() = Value::make(oi*pi);

			}
			else if (type == 2)
			{
				static const double PI = std::acos(-1);
				prp->value() = Value::make(oi*pi*PI);
			}
			else if (type == 3)
			{
				prp->value() = BooleanValue::make((oi*pi) % 13);
			}
			else if (type == 4)
			{
				ObjectPtr html = Object::make();
				html["lang"] = "en";
				html("head")["meta"] = {
					{ { "charset", "utf-8" } },
					{ { "name", "viewport" },{ "content", "width=device-width, initial-scale=1.0" } }
				};
				html("head")("title") = "SystemTest";
				html("head")("style") = "#itm { margin-left:25px; }";
				html("body")["details"] = "SystemTest";
				prp->value() = html;
			}
		}

		if ((oi % 10000) == 0)
		{
			PROCESS_MEMORY_COUNTERS_EX pmc;
			GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
			double virtualMemUsedByMe = pmc.PrivateUsage / 1024.0 / 1024.0;
			std::cout << "Object " << oi << ", Mem: " << virtualMemUsedByMe << "MB" << std::endl;
		}
	}
	fs::remove("../big_test.db3");
	DbHolder db("../big_test.db3");
	db.writeObject(root);
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

