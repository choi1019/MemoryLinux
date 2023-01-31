#pragma once

#include <22TestApplication/PTS2/typedef.h>
#define _PTC21_Id _GET_TESTCLASS_UID(_ETestComponent_PS2::_ePTC21)
#define _PTC21_Name "PTC21"
#include <22TestApplication/PTS2/PTC21/Config.h>

#include <21TestPlatform/TestCase/TestCase.h>

class PTC21 : public TestCase {
private:
	char* m_pMemeoryAllocated;
public:
	PTC21(
		int nClassId = _PTC21_Id,
		const char* pcClassName = _PTC21_Name)
		: TestCase(nClassId, pcClassName)
		, m_pMemeoryAllocated(nullptr)
	{
	}
	virtual ~PTC21() {
	}

	void Initialize() {
	}
	void Finalize() {
	}

	void Run() {
	}
};

