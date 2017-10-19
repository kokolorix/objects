#pragma once

#include "Object.h"

namespace obj
{
namespace db
{
void createDb(const String& dbName);
void deleteDb(const String& dbName);
ObjectPtr readObject(const String& dbName, IdType id);
IdType writeObject(const String& dbName, ObjectPtr object);
}
struct DbHolder
{
	DbHolder(const String& dbName) : _name(dbName)
	{
		db::createDb(_name);
	}
	virtual ~DbHolder()
	{
		db::deleteDb(_name);
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