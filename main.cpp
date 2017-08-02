
#include <iostream>
#include <fstream>
#include <string>
#include <thread>

#include "web_server/server.hpp"

#include "xml/rapidxml.hpp"
#include "xml/rapidxml_iterators.hpp"
using xml_document = rapidxml::xml_document<char>;
using xml_node = rapidxml::xml_node<char>;
using xml_attribute = rapidxml::xml_attribute<char>;
using node_iterator = rapidxml::node_iterator<char>;
using attribute_iterator = rapidxml::attribute_iterator<char>;

#include "json/json.hpp"
using json = nlohmann::json;

#include "Object.h"

void from_json(const json& j, PropertyPtr& p) 
{	

	p = Property::make(j.at("name").get<std::string>());

	if(j.is_boolean()) p->value() = BooleanValue::make(j.at("value").get<bool>());
	else if(j.is_number_integer()) p->value() = Int32Value::make(j.at("value").get<int32_t>());
	else if(j.is_string()) p->value() = StringValue::make(j.at("value").get<std::string>());
	else if (j.is_object())
	{
		ObjectPtr o = j;
		p->value() = ObjectValue::make(o);
	}
}

void from_json(const json& j, ObjectPtr& o)
{
	o = Object::make();
	for(const json& child : j)
	{
		PropertyPtr p = j;
		o->properties().push_back(p);
	}
}



int main()
{
	ValuePtr a2e = Value::make(42);
	PropertyPtr Name = Property::make("Name", StringValue::make("TheFirstObject"));
	PropertyPtr A2E = Property::make("AnswerToEverything", a2e);
	PropertyPtr Pi = Property::make("Pi", FloatValue::make(3.14f));
	PropertyPtr Id = Property::make("Id", UuIdValue::make(GenerateId()));
	ObjectPtr obj = Object::make(PropertySet{Name, A2E, Pi});

	//int32_t id = *Id->value();
	std::ostringstream os;

	std::ifstream is("D:\\Projects\\objects\\.vscode\\tasks.json");

	json j;
	is >> j;
	for (json& c : j)
	{
		ObjectPtr obj = j;
	}

	return 0;
}

/*
int main(int argc, char* argv[])
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
	std::string s = j.dump();
	try
	{
		// Check command line arguments.
		if (argc != 4)
		{
			std::cerr << "Usage: http_server <address> <port> <doc_root>\n";
			std::cerr << "  For IPv4, try:\n";
			std::cerr << "    receiver 0.0.0.0 80 .\n";
			std::cerr << "  For IPv6, try:\n";
			std::cerr << "    receiver 0::0 80 .\n";
			return 1;
		}
		// Initialise the server.
		http::server::server s(argv[1], argv[2], argv[3]);
		// Run the server until stopped.
		s.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "exception: " << e.what() << "\n";
	}
	return 0;
}
*/