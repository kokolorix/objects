#include "stdafx.h"
#include "SqLite.h"

#pragma warning(push)
#pragma warning(disable : 4065)
#include <sqlite/sqlite_modern_cpp.h>
#pragma warning(pop)

using namespace  sqlite;
using namespace obj;

IdType getValueTypeId(ValuePtr value);

IdType writeObjectToDb(database& db, ObjectPtr object);
IdType writePropertyToDb(database &db, IdType objectId, PropertyPtr property);
IdType writeValueToDb(database &db, IdType propertyId, ValuePtr value, IdType valueId = 0, IdType parentId = 0);

ObjectPtr readObjctFromDb(database& db, IdType id);
PropertyPtr readPropertyFromDb(database& db, IdType id);
ValuePtr readPropertyValueFromDb(database& db, IdType propertyId);
ValuePtr readValueFromDb(database& db, IdType id);

ObjectPtr obj::db::readObject(const Path & filePath, IdType id)
{
	database db(filePath.string());
	return readObjctFromDb(db, id);
}

IdType obj::db::writeObject(const Path & filePath, ObjectPtr object)
{
	database db(filePath.string());
	return writeObjectToDb(db, object);
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
	auto ps = db << "select id, value from properties where objectId = ? and name = ?;";
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
	IdType type = getValueTypeId(value);
	uint32_t version = 1;

	if (auto v = dynamic_pointer_cast<ObjectValue>(value))
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
	else
		valStr = value;


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
	}

	return valueId;
}

ObjectPtr readObjctFromDb(database & db, IdType id)
{	
	ObjectPtr obj;
	auto oQuery = db << "select type, version from object where id = ?;" << id;
	oQuery >> [&](unique_ptr<int> p, unique_ptr<int> v)
	{
		obj = Object::make(id);

	};

	if (obj)
	{
		auto pQuery = db << "select id from property where object = ?;" << id;
		pQuery >> [&](unique_ptr<int> p)
		{
			PropertyPtr prop = readPropertyFromDb(db, *p);
			obj->properties().push_back(prop);
		};
	}

	return obj;
}

PropertyPtr readPropertyFromDb(database & db, IdType propertyId)
{
	PropertyPtr prop;
	auto pQuery = db << "select name from property where id = ?;" << propertyId;
	pQuery >> [&](unique_ptr<String> p)
	{
		ValuePtr value = readPropertyValueFromDb(db, propertyId);
		prop = Property::make(*p, value);
	};
		
	return prop;
}

ValuePtr readPropertyValueFromDb(database & db, IdType propertyId)
{
	ValuePtr value;
	auto pQuery = db << "select id from value where parent is null and property = ?;" << propertyId;
	pQuery >> [&](unique_ptr<sqlite3_int64> i)
	{
		IdType id = static_cast<IdType>(*i);
		value = readValueFromDb(db, id);
	};
	return value;
}

ValuePtr readValueFromDb(database & db, IdType id)
{
	ValuePtr value;
	auto pQuery = db << "select type, value from value where id = ?;" << id;
	pQuery >> [&]( unique_ptr<sqlite3_int64> t, unique_ptr<String> v)
	{
		IdType typeId = static_cast<IdType>(*t);
		const String strValue = *v;
		if (typeId == 4)
			value = StringValue::make(strValue);
		else if (typeId == 5)
			value = BooleanValue::make(lexical_cast<bool>(strValue));
		else if (typeId == 6)
			value = Int32Value::make(lexical_cast<int32_t>(strValue));
		else if (typeId == 7)
			value = UInt32Value::make(lexical_cast<uint32_t>(strValue));
		else if (typeId == 8)
			value = FloatValue::make(std::stod(strValue));
			//value = FloatValue::make(lexical_cast<double>(strValue));
		else if (typeId == 9)
			value = UuIdValue::make(lexical_cast<UuId>(strValue));
		else if (typeId == 10)
		{
			IdType objectId = lexical_cast<IdType>(strValue);
			ObjectPtr obj = readObjctFromDb(db, objectId);
			value = ObjectValue::make(obj);
		}
		else if (typeId == 11)
		{
			auto vectorValue = VectorValue::make(ValuePtrVector());
			auto& valueVector = const_cast<ValuePtrVector&>(vectorValue->value());
			auto pQuery = db << "select id from value where parent = ?;" << id;
			pQuery >> [&](unique_ptr<sqlite3_int64> i)
			{	
				ValuePtr value = readValueFromDb(db, static_cast<IdType>(*i));
				valueVector.push_back(value);
			};
			value = vectorValue;
		}
		else if (typeId == 12)
			value = NothingValue<Unknown>::make();
	};
	return value;

}
IdType getValueTypeId(ValuePtr value)
{
	if (auto v = dynamic_pointer_cast<StringValue>(value))
		return 4;
	else if (auto v = dynamic_pointer_cast<BooleanValue>(value))
		return 5;
	else if (auto v = dynamic_pointer_cast<Int32Value>(value))
		return 6;
	else if (auto v = dynamic_pointer_cast<UInt32Value>(value))
		return 7;
	else if (auto v = dynamic_pointer_cast<FloatValue>(value))
		return 8;
	else if (auto v = dynamic_pointer_cast<UuIdValue>(value))
		return 9;
	else if (auto v = dynamic_pointer_cast<ObjectValue>(value))
		return 10;
	else if (auto v = dynamic_pointer_cast<VectorValue>(value))
		return 11;
	else /*if (auto v = dynamic_pointer_cast<NothingValue<>(value))*/
		return 12;
}
