// UnitTest.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Test.h"
using namespace obj;

int main(int argc, char* argv[])
{
	TestResult r = TestResult::Successful;
	if (argc)
	{
		for (int i = 1; i < argc; ++i)
		{
			r = runTests(argv[i]);
			if (r != TestResult::Successful)
				return static_cast<int>(r);
		}
	}
	else
	{
		r = runTests();
	}
	return static_cast<int>(r);
}

