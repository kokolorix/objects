#pragma once

#include <sstream>
#include <typeinfo>
#include <memory>
#include <functional>
namespace obj
{
using ::std::istringstream;
using ::std::ostringstream;
using ::std::unique_ptr;
using ::std::make_unique;
using ::std::shared_ptr;
using ::std::make_shared;
using ::std::dynamic_pointer_cast;
using ::std::function;

using String = std::string;
}

#include <boost/filesystem.hpp>
namespace obj
{
	namespace fs = boost::filesystem;
	using Path = fs::path;
}

#include <boost/format.hpp>
namespace obj
{
using Format = boost::format;
using boost::str;
}


namespace obj
{
// using Exception = std::exception;
struct Exception : public std::exception
{
	Exception(const char *msg) : _msg(msg) {}
	Exception(const String& msg) : _msg(msg) {}
	Exception(Format& format) : _msg(format.str()) {}
	//template<typename T, typename... Targs>
	//void Exception(Format& format, T value, Targs... Fargs) // recursive variadic function
	//{

		//for (; *format != '\0'; format++) {
		//	if (*format == '%') {
		//		std::cout << value;
		//		tprintf(format + 1, Fargs...); // recursive call
		//		return;
		//	}
		//	std::cout << *format;
		//}
	//}
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
	virtual bool operator < (const Thing& other) const
	{
		return this < &other;
	}
};

using ThingPtr = shared_ptr<Thing>;
inline bool operator < (ThingPtr x, ThingPtr y)
{
	return  (x.get() && y.get()) ? (*x < *y) : x.get() < y.get();
}
//inline bool operator > (ThingPtr x, ThingPtr y) {return }
inline bool operator == (ThingPtr x, ThingPtr y)
{
	return x.get() == y.get() || (x.get() && y.get() && !(*x < *y) && !(*y < *x));
}
//inline bool operator != (ThingPtr x, ThingPtr y) { return !(x == y); }


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

template<typename T>
struct MemberProperty
{
	using Getter = function<T()>;
	using Setter = function<T(const T&)>;

	MemberProperty(Getter g) : _getter(g) {}
	MemberProperty(Getter g, Setter s) : _getter(g), _setter(s) {}
	MemberProperty(T& m, Setter s) : _setter(s)
	{
		_getter = [&m]() { return m; };
	}
	MemberProperty(Getter g, T& m) : _getter(g)
	{
		_setter = [&m](const T& v) { return m = v; };
	}
	MemberProperty(T& m)
	{
		_getter = [&m]() { return m; };
		_setter = [&m](const T& v) { return m = v; };
	}
	operator T () const { return _getter(); }
	T operator = (const T& m) { return _setter(m); }
private:
	Getter _getter;
	Setter _setter;
};

}