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
	virtual operator String() const
	{
		std::ostringstream os;
		os << typeid(*this).name() << " (" << std::hex << this << ")";
		return os.str();
	}
};

using ThingPtr = std::shared_ptr<Thing>;
