#pragma once

#include <22TestApplication/PTS1/typedef.h>
#define _PTC13_Id _GET_TESTCLASS_UID(_ETestComponent_PS1::_ePTC13)
#define _PTC13_Name "PTC13"

#include <21TestPlatform/TestCase/TestCase.h>
#include <22TestApplication/PTS1/PTC13/DomainObject13.h>

class PTC13 : public TestCase {

private:
	DomainObject13* m_pDomainObject1;
	DomainObject13* m_pDomainObject2;
public:

	PTC13(
		unsigned nClassId = _PTC13_Id,
		const char* pcClassName = _PTC13_Name);
	~PTC13() override;

	void Initialize() override;
	void Finalize() override;
	void Run() override;
};

