#pragma once

#include "Test.h"

namespace obj
{
	namespace Test
	{
		struct JsonTests : public Case
		{
			virtual TestResult runTest() override;
		};
	}
}