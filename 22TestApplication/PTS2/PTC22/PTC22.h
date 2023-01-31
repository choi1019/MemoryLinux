#pragma once

#include <22TestApplication/PTS2/typedef.h>
#define _PTC22_Id _GET_TESTCLASS_UID(_ETestComponent_PS2::_ePTC22)
#define _PTC22_Name "PTC22"

#include <21TestPlatform/TestCase/TestCase.h>

class PTC22 : public TestCase {
private:

public:
	PTC22(
		unsigned nClassId = _PTC22_Id,
		const char* pcClassName = _PTC22_Name);
	~PTC22() override;

	void Initialize() override;
	void Finalize() override;

	void Run() override;
};

