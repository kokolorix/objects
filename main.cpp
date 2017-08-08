
#include <iostream>
#include <fstream>
#include <string>
#include <thread>

#include "web_server/server.hpp"

// #include "xml/rapidxml.hpp"
// #include "xml/rapidxml_iterators.hpp"
// using xml_document = rapidxml::xml_document<char>;
// using xml_node = rapidxml::xml_node<char>;
// using xml_attribute = rapidxml::xml_attribute<char>;
// using node_iterator = rapidxml::node_iterator<char>;
// using attribute_iterator = rapidxml::attribute_iterator<char>;


#include "Object.h"
#include "Json.h"

int main()
{
	ValuePtr a2e = Value::make(42);
	PropertyPtr Name = Property::make("Name", StringValue::make("TheFirstObject"));
	PropertyPtr A2E = Property::make("AnswerToEverything", a2e);
	PropertyPtr Pi = Property::make("Pi", FloatValue::make(3.14f));
	PropertyPtr Id = Property::make("Id", UuIdValue::make(GenerateId()));
	ObjectPtr obj = Object::make(PropertySet{Name, A2E, Pi});

	//int32_t id = *Id->value();

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