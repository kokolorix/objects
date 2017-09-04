#pragma once

#include "Object.h"

namespace obj
{
namespace db
{
ObjectPtrVector readObjects(const Path& filePath, const String& where = "");
ObjectPtr readObject(const Path& filePath, const String& where = "");
void writeObject(const Path& filePath, ObjectPtr object);
}
}