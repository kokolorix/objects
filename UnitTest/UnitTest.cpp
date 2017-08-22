// UnitTest.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Test.h"
using namespace obj;

int main()
{
    TestResult r = runTests();
    return static_cast<int>(r);
}

