#include "stdafx.h"
#include "SqLite.h"

#pragma warning(push)
#pragma warning(disable : 4065)
#include <sqlite/sqlite_modern_cpp.h>
#pragma warning(pop)

using namespace  sqlite;
using namespace obj;

bool checkDb(database& db, const String & dbName);

IdType getValueTypeId(ValuePtr value);

IdType writeObjectToDb(database& db, ObjectPtr object);
IdType writePropertyToDb(database &db, IdType objectId, PropertyPtr property);
IdType writeValueToDb(database &db, IdType propertyId, ValuePtr value, IdType valueId = 0, IdType parentId = 0);

ObjectPtr readObjctFromDb(database& db, IdType id);
PropertyPtr readPropertyFromDb(database& db, IdType id);
ValuePtr readPropertyValueFromDb(database& db, IdType propertyId);
ValuePtr readValueFromDb(database& db, IdType id);

ObjectPtr obj::db::readObject(const String & dbName, IdType id)
{
	database db(dbName);
	if(checkDb(db, dbName))
		return readObjctFromDb(db, id);
	return ObjectPtr();
}

IdType obj::db::writeObject(const String & dbName, ObjectPtr object)
{
	database db(dbName);
	checkDb(db, dbName);
	if(checkDb(db, dbName))
		return writeObjectToDb(db, object);
	return IdType();
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
	auto ps = db << "select id, value from PropertyList where ObjectId = ? and Name = ?;";
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

	property->id() = propertyId;

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
		std::const_pointer_cast<Value>(value)->id() = valueId;
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
			IdType itemId = writeValueToDb(db, propertyId, itemValue, 0, valueId);
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

bool checkDb(database& db, const String& dbName)
{
	static std::set<String> checkedDbs;
	auto res = checkedDbs.insert(dbName);
	if (res.second)
	{
		bool objectExists = false, propertyExists = false, valueExists = false;
		bool objectListExists = false, propertyListExists = false;
		{
			auto query = db << "SELECT name FROM sqlite_master WHERE type='table' AND name='object';";
			query >> [&](unique_ptr<String> n)
			{
				if(*n == "object")
					objectExists = true;
			};
			if (!objectExists)
			{
				auto ddl = db << 
				"CREATE TABLE object ("
				"	id      INTEGER PRIMARY KEY"
				"						 NOT NULL,"
				"	type    INTEGER REFERENCES object (id) ON DELETE NO ACTION"
				"														ON UPDATE NO ACTION"
				"														MATCH SIMPLE,"
				"	version INTEGER NOT NULL"
				"						 DEFAULT (1) "
				");"
				;
				ddl.execute();
				db << "begin;";
				db << "INSERT INTO object (type, version) VALUES (1, 1);";
				db << "INSERT INTO object (type, version) VALUES (1, 1);";
				db << "INSERT INTO object (type, version) VALUES (1, 1);";
				db << "INSERT INTO object (type, version) VALUES (2, 1);";
				db << "INSERT INTO object (type, version) VALUES (2, 1);";
				db << "INSERT INTO object (type, version) VALUES (2, 1);";
				db << "INSERT INTO object (type, version) VALUES (2, 1);";
				db << "INSERT INTO object (type, version) VALUES (2, 1);";
				db << "INSERT INTO object (type, version) VALUES (2, 1);";
				db << "INSERT INTO object (type, version) VALUES (2, 1);";
				db << "INSERT INTO object (type, version) VALUES (2, 1);";
				db << "INSERT INTO object (type, version) VALUES (2, 1);";				
				db << "commit;";
				objectExists = true;
			}
		}
		{
			auto query = db << "SELECT name FROM sqlite_master WHERE type='table' AND name='property';";
			query >> [&](unique_ptr<String> n)
			{
				if(*n == "property")
					propertyExists = true;
			};
			if (!propertyExists)
			{
				auto ddl = db << 
				"CREATE TABLE property ("
				"	id      INTEGER PRIMARY KEY,"
				"	name    STRING,"
				"	object  INTEGER REFERENCES object (id) ON DELETE CASCADE"
				"														ON UPDATE CASCADE"
				"														MATCH SIMPLE"
				"						 NOT NULL,"
				"	version INTEGER NOT NULL"
				"						 DEFAULT (1) "
				");"
				;
				ddl.execute();
				db << "begin;";
				db << "INSERT INTO property (name, object, version) VALUES ('Name', 1, 1);";
				db << "INSERT INTO property (name, object, version) VALUES ('Name', 2, 1);";
				db << "INSERT INTO property (name, object, version) VALUES ('Name', 3, 1);";
				db << "INSERT INTO property (name, object, version) VALUES ('Name', 4, 1);";
				db << "INSERT INTO property (name, object, version) VALUES ('Name', 5, 1);";
				db << "INSERT INTO property (name, object, version) VALUES ('Name', 6, 1);";
				db << "INSERT INTO property (name, object, version) VALUES ('Name', 7, 1);";
				db << "INSERT INTO property (name, object, version) VALUES ('Name', 8, 1);";
				db << "INSERT INTO property (name, object, version) VALUES ('Name', 9, 1);";
				db << "INSERT INTO property (name, object, version) VALUES ('Name', 10, 1);";
				db << "INSERT INTO property (name, object, version) VALUES ('Name', 11, 1);";
				db << "INSERT INTO property (name, object, version) VALUES ('Name', 12, 1);";
				db << "INSERT INTO property (name, object, version) VALUES ('Description', 1, 1);";
				db << "INSERT INTO property (name, object, version) VALUES ('Description', 2, 1);";
				db << "INSERT INTO property (name, object, version) VALUES ('Description', 3, 1);";
				db << "INSERT INTO property (name, object, version) VALUES ('Description', 4, 1);";
				db << "INSERT INTO property (name, object, version) VALUES ('Description', 5, 1);";
				db << "INSERT INTO property (name, object, version) VALUES ('Description', 6, 1);";
				db << "INSERT INTO property (name, object, version) VALUES ('Description', 7, 1);";
				db << "INSERT INTO property (name, object, version) VALUES ('Description', 8, 1);";
				db << "INSERT INTO property (name, object, version) VALUES ('Description', 9, 1);";
				db << "INSERT INTO property (name, object, version) VALUES ('Description', 10, 1);";
				db << "INSERT INTO property (name, object, version) VALUES ('Description', 11, 1);";
				db << "INSERT INTO property (name, object, version) VALUES ('Description', 12, 1);";
				db << "commit;";
				propertyExists = true;
			}
		}
		{
			auto query = db << "SELECT name FROM sqlite_master WHERE type='table' AND name='value';";
			query >> [&](unique_ptr<String> n)
			{
				if(*n == "value")
					valueExists = true;
			};
			if (!valueExists)
			{
				auto ddl = db << 
				"CREATE TABLE value ("
				"	id       INTEGER PRIMARY KEY,"
				"	type             REFERENCES object (id) ON DELETE CASCADE"
				"														 ON UPDATE CASCADE"
				"														 MATCH SIMPLE,"
				"	property         REFERENCES property (id) ON DELETE CASCADE"
				"															ON UPDATE CASCADE"
				"															MATCH SIMPLE,"
				"	value    STRING,"
				"	parent           REFERENCES value (id) ON DELETE CASCADE"
				"														ON UPDATE CASCADE"
				"														MATCH SIMPLE,"
				"	version  INTEGER DEFAULT (1) "
				");"
				;
				ddl.execute();
				db << "begin;";
				db << "INSERT INTO value (type, property, value, parent, version) VALUES (4, 1, 'BaseType', NULL, 1);";
				db << "INSERT INTO value (type, property, value, parent, version) VALUES (4, 2, 'ValueType', NULL, 1);";
				db << "INSERT INTO value (type, property, value, parent, version) VALUES (4, 3, 'ObjectType', NULL, 1);";
				db << "INSERT INTO value (type, property, value, parent, version) VALUES (4, 4, 'StringValue', NULL, 1);";
				db << "INSERT INTO value (type, property, value, parent, version) VALUES (4, 5, 'BooleanValue', NULL, 1);";
				db << "INSERT INTO value (type, property, value, parent, version) VALUES (4, 6, 'Int32Value', NULL, 1);";
				db << "INSERT INTO value (type, property, value, parent, version) VALUES (4, 7, 'UInt32Value', NULL, 1);";
				db << "INSERT INTO value (type, property, value, parent, version) VALUES (4, 8, 'FloatValue', NULL, 1);";
				db << "INSERT INTO value (type, property, value, parent, version) VALUES (4, 9, 'UuIdValue', NULL, 1);";
				db << "INSERT INTO value (type, property, value, parent, version) VALUES (4, 10, 'ObjectValue', NULL, 1);";
				db << "INSERT INTO value (type, property, value, parent, version) VALUES (4, 11, 'VectorValue', NULL, 1);";
				db << "INSERT INTO value (type, property, value, parent, version) VALUES (4, 12, 'NothingValue', NULL, 1);";
				db << "INSERT INTO value (type, property, value, parent, version) VALUES (4, 13, 'Basistyp für alle Typen', NULL, 1);";
				db << "INSERT INTO value (type, property, value, parent, version) VALUES (4, 14, 'Basistyp für alle Werttypen', NULL, 1);";
				db << "INSERT INTO value (type, property, value, parent, version) VALUES (4, 15, 'Basistyp für alle Objekttypen', NULL, 1);";
				db << "INSERT INTO value (type, property, value, parent, version) VALUES (4, 16, 'Werttyp für Strings', NULL, 1);";
				db << "INSERT INTO value (type, property, value, parent, version) VALUES (4, 17, 'Werttyp für Booleans', NULL, 1);";
				db << "INSERT INTO value (type, property, value, parent, version) VALUES (4, 18, 'Werttyp für vorzeichenbehaftete 32bit Integers', NULL, 1);";
				db << "INSERT INTO value (type, property, value, parent, version) VALUES (4, 19, 'Werttyp für vorzeichenlose 32bit Integers', NULL, 1);";
				db << "INSERT INTO value (type, property, value, parent, version) VALUES (4, 20, 'Werttyp für Fliesskommazahlen', NULL, 1);";
				db << "INSERT INTO value (type, property, value, parent, version) VALUES (4, 21, 'Werttyp für GUIDs (Global unique identifier)', NULL, 1);";
				db << "INSERT INTO value (type, property, value, parent, version) VALUES (4, 22, 'Werttyp für Objketreferenzen', NULL, 1);";
				db << "INSERT INTO value (type, property, value, parent, version) VALUES (4, 23, 'Werttyp für Arrays von Werten', NULL, 1);";
				db << "INSERT INTO value (type, property, value, parent, version) VALUES (4, 24, 'Werttyp für Nichts', NULL, 1);";
				db << "commit;";
				valueExists = true;
			}
			{
				auto query = db << "SELECT name FROM sqlite_master WHERE type='view' AND name='PropertyList';";
				query >> [&](unique_ptr<String> n)
				{
					if (*n == "PropertyList")
						propertyListExists = true;
				};
				if (!propertyListExists)
				{
					auto ddl = db << 
					"CREATE VIEW PropertyList AS "
					"	SELECT p.id AS Id,"
					"		 p.name AS Name,"
					"		 v.value AS Value,"
					"		 vt.value AS ValueType,"
					"		 v.id AS ValueId,"
					"		 v.type AS ValueTypeId,"
					"		 ovt.value AS ObjectType,"
					"		 p.object AS ObjectId,"
					"		 o.type AS ObjectTypeId"
					"	FROM property p"
					"		 LEFT JOIN"
					"		 value v ON p.id = v.property"
					"		 LEFT JOIN"
					"		 property pt ON pt.object = v.type AND "
					"							 pt.name = 'Name'"
					"		 LEFT JOIN"
					"		 value vt ON pt.id = vt.property"
					"		 LEFT JOIN"
					"		 object o ON o.id = p.object"
					"		 LEFT JOIN"
					"		 property opt ON opt.object = o.type AND "
					"							  opt.name = 'Name'"
					"		 LEFT JOIN"
					"		 value ovt ON opt.id = ovt.property"
					";"
					;
					ddl.execute();
					propertyListExists = true;
				}
			}
			{
				auto query = db << "SELECT name FROM sqlite_master WHERE type='view' AND name='ObjectList';";
				query >> [&](unique_ptr<String> n)
				{
					if (*n == "ObjectList")
						objectListExists = true;
				};
				if (!objectListExists)
				{
					auto ddl = db <<
						"CREATE VIEW ObjectList AS "
						"	SELECT o.id AS Id,"
						"		 p1.Value AS Name,"
						"		 p2.Value AS Description,"
						"		 o.type AS TypeId,"
						"		 p3.Value AS TypeName"
						"	FROM object AS o"
						"		 LEFT OUTER JOIN"
						"		 PropertyList AS p1 ON o.id = p1.ObjectId AND "
						"									p1.Name = 'Name'"
						"		 LEFT OUTER JOIN"
						"		 PropertyList AS p2 ON o.id = p2.ObjectId AND "
						"									p2.Name = 'Description'"
						"		 LEFT OUTER JOIN"
						"		 PropertyList AS p3 ON o.type = p3.ObjectId AND "
						"									p3.Name = 'Name'"
						";"
						;
					ddl.execute();
					objectListExists = true;
				}
			}
		}

		return  objectExists && propertyExists && valueExists && objectListExists && propertyListExists;
	}
	else
		return true;
}
