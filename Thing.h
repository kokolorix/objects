#pragma once

#include <sstream>
#include <typeinfo>
#include <memory>

using String = std::string;
// using Exception = std::exception;
class Exception : public std::exception
{
 public:
	Exception(const char *msg) : _msg(msg) {}
	virtual const char *what() const noexcept override { return _msg.c_str(); }

 private:
	String _msg;
};
using NotImplementetException = Exception;

class Thing
{
 public:
	virtual ~Thing() {}
	virtual String toString() const
	{
		std::ostringstream os;
		os << typeid(*this).name() << " (" << std::hex << this << ")";
		return os.str();
	}
	virtual String to_json() const { throw NotImplementetException(__func__); };
	virtual void from_json(const String& jsonStr) const { throw NotImplementetException(__func__); };
};

using ThingPtr = std::shared_ptr<Thing>;
