#pragma once

#include <22TestApplication/typedef.h>
#define _PTS2_Id _GET_TESTLAYER_UID(_ELayer_TestApplication::_ePTS2)
#define _PTS2_Name "PTS2"

#include <21TestPlatform/TestCase/TestSuite.h>
#include <13PTechnical/PMemoryManager/PMemory.h>

class PTS2: public TestSuite {
public:
	PTS2(
		unsigned nClassId = _PTS2_Id,
		const char* pClassName = _PTS2_Name);
	~PTS2() override;

	void Initialize() override;
	void Finalize() override;
};

