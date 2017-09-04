#include "stdafx.h"
#include "SqLite.h"

#pragma warning(push)
#pragma warning(disable : 4065)
#include <sqlite_modern_cpp.h>
#pragma warning(pop)

using namespace  sqlite;
using namespace obj;

ObjectPtrVector obj::db::readObjects(const Path & filePath, const String & where)
{
	database db(filePath.string());

	int count = 0;
	db << "select count(*) from object where name='RootType'" >> count;
	if (count == 0)
	{
		db << "insert into object (name,type) values (?,?);" << u"RootType" << 9;
	}
	return ObjectPtrVector();
}

ObjectPtr obj::db::readObject(const Path & filePath, const String & where)
{
	database db(filePath.string());

	return ObjectPtr();
}

int32_t writeObjectToDb(database& db, ObjectPtr object);
int32_t writePropertyToDb(database &db, int32_t objectId, PropertyPtr property);
int32_t writeValueToDb(database &db, int32_t propertyId, ValuePtr value, int32_t parentId = 0);

void obj::db::writeObject(const Path & filePath, ObjectPtr object)
{
	database db(filePath.string());
	int32_t id = writeObjectToDb(db, object);
}

int32_t writeObjectToDb(database& db, ObjectPtr object)
{
	Int32ValuePtr pId = dynamic_pointer_cast<Int32Value>(object["id"]);
	int32_t id = 0;
	if (pId)
		id = pId->value();
	else
		db << "select max(id) from object" >> id;

	String name = object["name"];

	id = (id > 1000) ? id + 1 : 1001;
	for (bool inserted = false; !inserted; id++)
	{
		try
		{
			db << "insert into object (id,name,type) values (?,?,?);" << id << name << 3;
			inserted = true;
		}
		catch (sqlite::errors::constraint_primarykey&) {}
	}

	for (PropertyPtr p : object->properties())
	{
		int32_t propertyId = writePropertyToDb(db, id, p);
	}

	return id;
}

int32_t writePropertyToDb(database &db, int32_t objectId, PropertyPtr property)
{
	ValuePtr value = property->value();
	String name = property->name();
	int32_t version = 1;

	int32_t id = 0;
	db << "select max(id) from property" >> id;
	id += 1;

	for (bool inserted = false; !inserted; id++)
	{
		try
		{
			db << "insert into property (id,name,object,version) values (?,?,?,?);"
				<< id << name << objectId << version;
			inserted = true;
		}
		catch (sqlite::errors::constraint_primarykey &)
		{
		}
	}

	int32_t valueId = writeValueToDb(db, id, value);

	return id;
}

int32_t writeValueToDb(database &db, int32_t propertyId, ValuePtr value, int32_t parentId /*= 0*/)
{
	String valStr;
	VectorValuePtr array;
	int32_t type, version = 1;
	if (auto v = dynamic_pointer_cast<StringValue>(value))
	{
		type = 4;
		valStr = *v;
	}
	else if (auto v = dynamic_pointer_cast<BooleanValue>(value))
	{
		type = 5;
		valStr = *v;
	}
	else if (auto v = dynamic_pointer_cast<Int32Value>(value))
	{
		type = 6;
		valStr = *v;
	}
	else if (auto v = dynamic_pointer_cast<UInt32Value>(value))
	{
		type = 7;
		valStr = *v;
	}
	else if (auto v = dynamic_pointer_cast<FloatValue>(value))
	{
		type = 8;
		valStr = *v;
	}
	else if (auto v = dynamic_pointer_cast<UuIdValue>(value))
	{
		type = 9;
		valStr = *v;
	}
	else if (auto v = dynamic_pointer_cast<ObjectValue>(value))
	{
		type = 10;
		int32_t valId = writeObjectToDb(db, v->value());
		valStr = boost::lexical_cast<String>(valId);
	}
	else if (auto v = dynamic_pointer_cast<VectorValue>(value))
	{
		type = 11;
		array = v;
		valStr = boost::lexical_cast<String>(v->value().size());
	}

	std::unique_ptr<int32_t> parent;
	if (parentId)
		parent = std::make_unique<int32_t>(parentId);

	int32_t id = 0;
	db << "select max(id) from value" >> id;
	id += 1;

	for (bool inserted = false; !inserted; id++)
	{
		try
		{
			db << "insert into value (id,type,value,property,parent,version) values (?,?,?,?,?,?);"
				<< id  << type << valStr << propertyId << parent << version;
			inserted = true;
		}
		catch (sqlite::errors::constraint_primarykey &)
		{
		}
	}

	if (array)
	{
		for (ValuePtr v : array->value())
		{
			int32_t elId = writeValueToDb(db, propertyId, v, id);
		}

	}

	return id;
}

