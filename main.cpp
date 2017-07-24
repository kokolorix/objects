
#include <iostream>
#include <string>
#include <thread>
#include <boost/asio.hpp>
#include "web_server/server.hpp"
#include "Object.h"

int main()
{
	ValuePtr a2e = Value::make(42);
	PropertyPtr Name = Property::make("Name", StringValue::make(L"TheFirstObject"));
	PropertyPtr A2E = Property::make("AnswerToEverything", a2e);
	PropertyPtr Pi = Property::make("Pi", FloatValue::make(3.14));
	ObjectPtr obj = Object::make(PropertySet{Name, A2E, Pi});

	return 0;
}

/*
int main(int argc, char* argv[])
{
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