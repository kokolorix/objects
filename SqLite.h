#pragma once

#include "Object.h"

namespace obj
{
namespace db
{
ObjectPtr readObject(const Path& filePath, IdType id);
IdType writeObject(const Path& filePath, ObjectPtr object);
}
}