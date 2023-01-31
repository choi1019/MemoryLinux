#pragma once

#include <22TestApplication/PTS1/typedef.h>
#define _PTC12_Id _GET_TESTCLASS_UID(_ETestComponent_PS1::_ePTC12)
#define _PTC12_Name "PTC12"

#include <21TestPlatform/TestCase/TestCase.h>
#include <22TestApplication/PTS1/PTC12/DomainObject12.h>

class PTC12 : public TestCase {
private:
	DomainObject12* m_pDomainObject1;
	DomainObject12* m_pDomainObject2;

public:
	PTC12(
		unsigned nClassId = _PTC12_Id,
		const char* pcClassName = _PTC12_Name);
	~PTC12() override;
	
	void Initialize() override;
	void Finalize() override;
	void Run() override;
};

