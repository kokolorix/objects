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
	return ObjectPtr();
}

void obj::db::writeObject(const Path & filePath, ObjectPtr object)
{
	int32_t id = *object->at("id");
	database db(filePath.string());

}
