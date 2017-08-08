#pragma once

#include <sstream>
#include <typeinfo>
#include <memory>

using String = std::string;
// using Exception = std::exception;
struct Exception : public std::exception
{
	Exception(const char *msg) : _msg(msg) {}
	virtual const char *what() const noexcept override { return _msg.c_str(); }

 private:
	String _msg;
};
struct NotImplementetException : public Exception {using Exception::Exception;};
struct NotFoundException : public Exception {using Exception::Exception;};
struct ImpossibleCastException : public Exception {using Exception::Exception;};

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
};

using ThingPtr = std::shared_ptr<Thing>;
