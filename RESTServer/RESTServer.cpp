// RESTServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "web_server\server.hpp"
#include "web_server\request_handler.hpp"
#include "web_server\mime_types.hpp"
using namespace http::server;

#include "Object.h"
using namespace obj;

int main(int argc, char* argv[])
{
	server s("127.0.0.1", "8080", ".");
	request_handler::get_extern_handler("/tv.html").connect([](const request& req, reply& rep)
	{
		char buf[] = 
			R"htm(<!doctype html>)htm"
			R"htm(<html lang = "en">)htm"
			R"htm(<head>)htm"
			R"htm(<meta charset = "utf-8">)htm"
			R"htm(<meta name = "viewport" content = "width=device-width, initial-scale=1.0">)htm"
			R"htm(<title>Titel</title>)htm"
			R"htm(</head>)htm"
			R"htm(<body>)htm"
			R"htm(	<details>)htm"
			R"htm(		<summary>1. Node</summary>)htm"
			R"htm(		<div style="margin-left:25px">1.1 Item</div>)htm"
			R"htm(		<div style="margin-left:25px">1.2 Item</div>)htm"
			R"htm(		<details style="margin-left:25px">)htm"
			R"htm(			<summary>1.3 Node</summary>)htm"
			R"htm(			<div style="margin-left:25px">1.3.1 Item</div>)htm"
			R"htm(			<div style="margin-left:25px">1.3.2 Item</div>)htm"
			R"htm(		</details>)htm"
			R"htm(	</details>)htm"
			R"htm(	<details>)htm"
			R"htm(		<summary>2. Node</summary>)htm"
			R"htm(		<div style="margin-left:25px">2.1 Item</div>)htm"
			R"htm(		<div style="margin-left:25px">2.2 Item</div>)htm"
			R"htm(	</details>)htm"
			R"htm(	<details>)htm"
			R"htm(		<summary>3. Node</summary>)htm"
			R"htm(		<div style="margin-left:25px">3.1 Item</div>)htm"
			R"htm(	</details>)htm"
			R"htm(</body>
)htm"
			;



		rep.content.append(buf, sizeof(buf));
		rep.headers.resize(2);
		rep.headers[0].name = "Content-Length";
		rep.headers[0].value = std::to_string(rep.content.size());
		rep.headers[1].name = "Content-Type";
		rep.headers[1].value = mime_types::extension_to_type("html");
		rep.status = reply::ok;
	});
	s.run();
	return 0;
}

