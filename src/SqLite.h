#pragma once

#include "Object.h"

namespace obj
{
namespace db
{
ObjectPtr readObject(const String& dbName, IdType id);
IdType writeObject(const String& dbName, ObjectPtr object);
}
}