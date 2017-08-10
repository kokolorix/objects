#pragma once

#include "Object.h"
#include <boost/filesystem.hpp>
#include "json/json.hpp"

namespace obj
{
   namespace fs = boost::filesystem;
}

namespace obj
{
using Path = fs::path;
using Json = nlohmann::json;

namespace js
{
ObjectPtr readJson(const Json& j);
ObjectPtr readFile(const Path& filePath);
ObjectPtr readString(const String& jsonString);
Json writeJson(ObjectPtr object);
void writeFile(const Path& filePath, ObjectPtr object);
String writeString(ObjectPtr object);
}//json
}//obj
