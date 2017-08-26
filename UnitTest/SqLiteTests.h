#pragma once

#include "Test.h"

namespace obj
{
	namespace Test
	{
		struct SqLiteTests : public Case
		{
			virtual TestResult runTest() override;
		};
	}
}