#pragma once

#include "Object.h"

namespace obj
{
namespace db
{
void initializeDb(const String& dbName);
void removeDb(const String& dbName);
ObjectPtr readObject(const String& dbName, IdType id);
IdType writeObject(const String& dbName, ObjectPtr object);
}
struct DbHolder
{
	DbHolder(const String& dbName) : _name(dbName)
	{
		db::initializeDb(_name);
	}
	virtual ~DbHolder()
	{
		db::removeDb(_name);
	}
	ObjectPtr readObject(IdType id)
	{
		return db::readObject(_name, id);
	}
	IdType writeObject(ObjectPtr object)
	{
		return db::writeObject(_name, object);
	}
private:
	String _name;
};
}