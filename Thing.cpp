#include "Thing.h"
#include <map>
using namespace obj;

struct ThingFactory::Impl
{
	using CreatorMap = std::map<String, ThingCreator>;
	CreatorMap _creatorMap;
	ThingCreator& operator[](const String& typeName);
	ThingCreator erase(const String& typeName);
	ThingPtr create(const String& typeName);
};

ThingFactory::ThingFactory()
	: _pimpl(std::make_unique<Impl>())
{}

ThingFactory & ThingFactory::instance()
{
	static ThingFactory singleton;
	return singleton;
}

ThingCreator& ThingFactory::operator [] (const String & typeName)
{
	return _pimpl->operator[](typeName);
}

ThingCreator ThingFactory::erase(const String & typeName)
{
	return _pimpl->erase(typeName);
}

ThingPtr ThingFactory::create(const String & typeName)
{
	return _pimpl->create(typeName);
}

ThingCreator & ThingFactory::Impl::operator[](const String & typeName)
{
	return _creatorMap[typeName];
}

ThingCreator ThingFactory::Impl::erase(const String & typeName)
{
	auto it = _creatorMap.find(typeName);
	if (it != _creatorMap.end())
	{
		ThingCreator creator = it->second;
		_creatorMap.erase(it);
		return creator;
	}
	return ThingCreator();
}

ThingPtr ThingFactory::Impl::create(const String & typeName)
{
	auto it = _creatorMap.find(typeName);
	if (it != _creatorMap.end())
	{
		ThingCreator creator = it->second;
		return creator();
	}
	return ThingPtr();
}
