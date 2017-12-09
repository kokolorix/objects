
#include "stdafx.h"
#include <cmath>
#include "windows.h"
#include "psapi.h"

#include "ThreadTests.h"
using namespace obj;

#include "Thread.h"
#include "Value.h"
#include "Json.h"

namespace
{
	bool test = Test::addTestCase(make_shared<Test::ThreadTests>(), "ThreadTests");
}

namespace threadT
{
	void simpleTest();
}

obj::TestResult obj::Test::ThreadTests::runTest()
{
	using namespace threadT;

	simpleTest();

	return TestResult::Successful;
}

void threadT::simpleTest()
{
	ThreadPtr thread1 = Thread::Pool["Hoppla"];
	ThreadPtr thread2 = Thread::Pool[generateId()];
	thread1->start();
	thread1->call([thread1]() {OutputStream() << (String&)thread1->Name; });
	thread1->stop();
	thread1->join();
}

