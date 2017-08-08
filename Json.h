#pragma once

#include "Object.h"
#include <boost/filesystem.hpp>
#include "json/json.hpp"


namespace obj
{
using Path = boost::filesystem::path;
using json = nlohmann::json;

namespace serialize_json
{
ObjectPtr ReadFromFile(const Path& filePath);
ObjectPtr ReadFromString(const String& jsonString);
void WriteToFile(const Path& filePath, ObjectPtr object);
String WriteToString(ObjectPtr object);
}
}
