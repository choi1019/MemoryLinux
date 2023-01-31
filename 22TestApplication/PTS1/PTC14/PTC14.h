#pragma once

#include <22TestApplication/PTS1/typedef.h>
#define _PTC14_Id _GET_TESTCLASS_UID(_ETestComponent_PS1::_ePTC14)
#define _PTC14_Name "PTC14"

#include <21TestPlatform/TestCase/TestCase.h>
#include <22TestApplication/PTS1/PTC14/DomainObject14.h>

class PTC14 : public TestCase {

private:
	DomainObject14* m_pDomainObject1;
	DomainObject14* m_pDomainObject2;

public:
	PTC14(
		unsigned nClassId = _PTC14_Id, 
		const char* pcClassName = _PTC14_Name);
	~PTC14() override;
	
	void Initialize() override;
	void Finalize() override;
	void Run() override;
};

