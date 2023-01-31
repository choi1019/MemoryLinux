#pragma once

#include <22TestApplication/PTS2/typedef.h>
#define _PTC22_Id _GET_TESTCLASS_UID(_ETestComponent_PS2::_ePTC22)
#define _PTC22_Name "PTC22"
#include <22TestApplication/PTS2/PTC22/Config.h>

#include <21TestPlatform/TestCase/TestCase.h>

class PTC22 : public TestCase {
private:
	char* m_pMemeoryAllocated;
public:
	PTC22(
		int nClassId = _PTC22_Id,
		const char* pcClassName = _PTC22_Name)
		: TestCase(nClassId, pcClassName)
		, m_pMemeoryAllocated(nullptr)
	{
	}
	virtual ~PTC22() {
	}
	void Initialize() {
		TestCase::Initialize();

	}

	void Finalize() {
		TestCase::Finalize();
	}

	void Run() {
	}
};

