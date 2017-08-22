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
