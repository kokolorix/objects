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

void obj::db::writeObject(const Path & filePath, ObjectPtr object)
{
	database db(filePath.string());

	Int32ValuePtr pId = dynamic_pointer_cast<Int32Value>(object["id"]);
	int id = 0;
	if (pId)
		id = pId->value();
	else
		db << "select max(id) from object" >> id;

	String name = object["name"];

	id = (id > 1000) ? id + 1 : 1001;
	db << "insert into object (id,name,type) values (?,?,?);" << id << name << 3;


}
