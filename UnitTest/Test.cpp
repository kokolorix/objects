
#include "stdafx.h"
#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <regex>


using std::endl;

#include "Test.h"
using namespace obj;


namespace
{
using CaseMap = std::map<String, obj::Test::CasePtr>;
CaseMap& getCaseMap()
{
	static CaseMap caseMap;
	return caseMap;
}

struct NameChecker
{
	NameChecker(const String& pattern) : _pattern(pattern) {}
	bool operator()(const String& name);
	String _pattern;
};


bool NameChecker::operator()(const String & name)
{
	if (_pattern == "*")
		return true;

	if(_pattern == name)
		return true;

	std::regex rx(_pattern);
	if (std::regex_match(name, rx))
		return true;
	else
		return false;
}

}



obj::TestResult obj::runTests(const String& testPattern /*= "*"*/)
{

	NameChecker check(testPattern);
	OutputStream out;
	for (CaseMap::value_type& entry : getCaseMap())
	{
		const String& name = entry.first;
		Test::CasePtr pTest = entry.second;
		if (check(name))
		{
			try
			{
				pTest->buildUp();

				switch (pTest->runTest())
				{
				case TestResult::Successful:
					out << "Test " << name << " successfully completed" << std::endl;
					break;
				case TestResult::Failed:
					out << "Test " << name << " failed" << std::endl;
					break;
				default:
					out << "Test " << name << " failed with unexpected result" << std::endl;
					break;
				}

				pTest->breakDown();
			}
			catch (std::exception& e)
			{
				out << "Test " << name << " failed: " << e.what() << std::endl;
				return TestResult::Exception;
			}
			catch (...)
			{
				out << "Test " << name << " failed with unexpected exception" << std::endl;
				return TestResult::Unexpected;
			}
		}
	}
	return TestResult::Successful;
}


bool obj::Test::addTestCase(CasePtr pTest, const String& name)
{
	CaseMap::_Pairib res = getCaseMap().insert(make_pair(name, pTest));
	return res.second;
}
