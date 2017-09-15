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

IdType writeObjectToDb(database& db, ObjectPtr object);
IdType writePropertyToDb(database &db, IdType objectId, PropertyPtr property);
IdType writeValueToDb(database &db, IdType propertyId, ValuePtr value, IdType valueId=0, IdType parentId = 0);

void obj::db::writeObject(const Path & filePath, ObjectPtr object)
{
	database db(filePath.string());
	IdType id = writeObjectToDb(db, object);
}

IdType writeObjectToDb(database& db, ObjectPtr object)
{
	IdType id = object->id();

	if (id)
	{
		db << "update object (type) values (?) where id = ?;" << 3 << id;
	}
	else
	{
		db << "insert into object (type) values (?);" << 3;
		id = static_cast<IdType>(db.last_insert_rowid());
		object->id() = id;
	}

	for (PropertyPtr p : object->properties())
	{
		IdType propertyId = writePropertyToDb(db, id, p);
	}

	return id;
}

IdType writePropertyToDb(database &db, IdType objectId, PropertyPtr property)
{
	ValuePtr value = property->value();
	String name = property->name();
	IdType version = 1;

	IdType propertyId(0), valueId(0);
	auto ps = db << "select id, value from properties where object = ? and name = ?;";
	ps << objectId << name;
	ps >> [&propertyId, &valueId](unique_ptr<int> p, unique_ptr<int> v)
	{ 
		if(p)
			propertyId =*p;
		if (v)
			valueId = *v;
	};
	if (propertyId)
	{
		db << "update property (name,object,version) values (?,?,?) where id = ?;"
			<< name << objectId << version << propertyId;
	}
	else
	{
		db << "insert into property (name,object,version) values (?,?,?);"
			<< name << objectId << version;
		propertyId = static_cast<IdType>(db.last_insert_rowid());
	}

	IdType v_id = writeValueToDb(db, propertyId, value, valueId);

	return propertyId;
}

IdType writeValueToDb(database &db, IdType propertyId, ValuePtr value, IdType valueId /*=0*/, IdType parentId /*= 0*/)
{
	String valStr;
	VectorValuePtr array;
	IdType type, version = 1;
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
		IdType valId = writeObjectToDb(db, v->value());
		valStr = boost::lexical_cast<String>(valId);
	}
	else if (auto v = dynamic_pointer_cast<VectorValue>(value))
	{
		type = 11;
		array = v;
		valStr = boost::lexical_cast<String>(v->value().size());
	}

	std::unique_ptr<IdType> parent;
	if (parentId)
		parent = std::make_unique<IdType>(parentId);

	if (valueId)
	{
		db << "update value (type,value,property,parent,version) values (?,?,?,?,?) where id = ?;"
			  << type << valStr << propertyId << parent << version << valueId;
	}
	else
	{
		db << "insert into value (type,value,property,parent,version) values (?,?,?,?,?);"
			<< type << valStr << propertyId << parent << version;
		valueId = static_cast<IdType>(db.last_insert_rowid());
	}

	if (array)
	{
		ValuePtrVector v = array->value();
		IdType itemId(0);
		size_t i = 0;
		db << "select id from value where parent = ?;" << valueId >> [&](unique_ptr<int> id)
		{
			if (i < v.size())
			{
				ValuePtr itemValue = v[i];
				IdType elId = writeValueToDb(db, propertyId, itemValue, *id, valueId);
			}
			else
			{
				db << "delete from value where id = ?;" << *id;
			}
			i++;
		};
		for (; i < v.size(); i++)
		{
			ValuePtr itemValue = v[i];
			IdType elId = writeValueToDb(db, propertyId, itemValue, 0, valueId);
		}

		//for (ValuePtr v : array->value())
		//{
		//	IdType elId = writeValueToDb(db, propertyId, v, valueId);
		//}

	}

	return valueId;
}

