#pragma once

#include "Object.h"
#include "json/json.hpp"

namespace obj
{
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