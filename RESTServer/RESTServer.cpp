// RESTServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "web_server\server.hpp"

int main(int argc, char* argv[])
{
	http::server::server s("127.0.0.1", "8080", ".");
	s.run();
	return 0;
}

