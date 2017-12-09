#pragma once

#include <sstream>
#include <iostream>
#include <typeinfo>
#include <memory>
#include <functional>
#include <windows.h>

namespace obj
{
	using ::std::istringstream;
	using ::std::ostringstream;
	using ::std::shared_ptr;
	using ::std::make_shared;

	using String = std::string;
}

namespace obj
{
	enum struct TestResult {
		Successful, Failed, Exception, Unexpected
	};

	TestResult runTests(const String& testPattern = "*");

	namespace Test
	{
		struct Case
		{
			virtual ~Case() = default;
			virtual void buildUp() {}
			virtual TestResult runTest() = 0;
			virtual void breakDown() {}
		};
		using CasePtr = std::shared_ptr<Case>;
		bool addTestCase(CasePtr pTest, const String& name);
	}
}
namespace obj
{
	template<typename Elem>
	inline void flush(std::basic_string<Elem>& msg)
	{
		if (msg.empty())
			return;

		std::cout << msg << std::endl;
		if (::IsDebuggerPresent())
		{
			OutputDebugStringA(msg.c_str());
			OutputDebugStringA("\n");
		}
	}

	template<>
	inline void flush(std::basic_string<wchar_t>& msg)
	{
		if (msg.empty())
			return;

		std::wcout << msg << std::endl;
		if (::IsDebuggerPresent())
		{
			OutputDebugStringW(msg.c_str());
			OutputDebugStringW(L"\n");
		}
	}

	template<typename Elem>
	class outputbuf : public std::basic_streambuf<Elem>
	{
	private:
		std::basic_string<Elem> msg;	///< buffer for current log message

		void flushMsg()
		{
			if (msg.length() > 0)
			{
				flush(msg);
				msg.erase();	// erase message buffer
			}
		}

	public:
		outputbuf() { };
		~outputbuf() { flushMsg(); }

	protected:
		virtual int_type overflow(int_type c)
		{
			std::basic_ostream<Elem> tmp(this);
			if (c == tmp.widen('\n'))
			{
				flushMsg();
			}
			else
			{
				msg += c;
			}
			return c == -1 ? -1 : ' ';
		}

	private:
		outputbuf(outputbuf const&);                 // disallow copy construction
		void operator= (outputbuf const&);           // disallow copy assignment
	};

	template<typename Elem>
	class output_stream
		: private outputbuf<Elem>
		, public std::basic_ostream<Elem>
	{
	public:
		explicit output_stream() : std::basic_ostream<Elem>(this) { }
	};

	using OutputStream = output_stream<char>;
}