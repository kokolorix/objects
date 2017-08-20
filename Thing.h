#pragma once

#include <sstream>
#include <typeinfo>
#include <memory>
#include <functional>
namespace obj
{
using ::std::istringstream;
using ::std::ostringstream;
using ::std::shared_ptr;

using String = std::string;
}

#include <boost/filesystem.hpp>
namespace obj
{
	namespace fs = boost::filesystem;
	using Path = fs::path;
}


namespace obj
{
// using Exception = std::exception;
struct Exception : public std::exception
{
	Exception(const char *msg) : _msg(msg) {}
	virtual const char *what() const noexcept override { return _msg.c_str(); }

private:
	String _msg;
};
struct NotImplementetException : public Exception { using Exception::Exception; };
struct NotFoundException : public Exception { using Exception::Exception; };
struct ImpossibleCastException : public Exception { using Exception::Exception; };

class Thing
{
public:
	virtual ~Thing() {}
	virtual String toString() const
	{
		ostringstream os;
		os << typeid(*this).name() << " (" << std::hex << this << ")";
		return os.str();
	}
};

using ThingPtr = shared_ptr<Thing>;

using ThingCreator = std::function<ThingPtr(void)>;
class ThingFactory
{
public:
	static ThingFactory& instance();
	ThingCreator& operator [] (const String& typeName);
	ThingCreator erase(const String& typeName);
	ThingPtr create(const String& typeName);

private:
	ThingFactory();
	struct Impl;
	std::unique_ptr<Impl> _pimpl;
};
}