#pragma once

#include "Test.h"

namespace obj
{
	namespace Test
	{
		struct SystemTests : public Case
		{
			virtual TestResult runTest() override;
		};
	}
}