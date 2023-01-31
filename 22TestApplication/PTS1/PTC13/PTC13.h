#pragma once

#include <22TestApplication/PTS1/typedef.h>
#define _PTC13_Id _GET_TESTCLASS_UID(_ETestComponent_PS1::_ePTC13)
#define _PTC13_Name "PTC13"
#include <22TestApplication/PTS1/PTC13/Config.h>

#include <21TestPlatform/TestCase/TestCase.h>
#include <22TestApplication/PTS1/PTC13/DomainObject13.h>

class PTC13 : public TestCase {
private:
	DomainObject13* m_pDomainObject1;
	DomainObject13* m_pDomainObject2;
public:
	PTC13(
		int nClassId = _PTC13_Id,
		const char* pcClassName = _PTC13_Name)
		: TestCase(nClassId, pcClassName)
		, m_pDomainObject1(nullptr)
		, m_pDomainObject2(nullptr)
	{
		m_pDomainObject1 = new("PTC13::DomainObject1") DomainObject13();
		BaseObject::s_pMemory->Show("new PTC13::DomainObject1");
		m_pDomainObject2 = new("PTC13::DomainObject2") DomainObject13();
		BaseObject::s_pMemory->Show("new PTC13::DomainObject2");
	}
	virtual ~PTC13() {
		delete m_pDomainObject1;
		BaseObject::s_pMemory->Show("delete PTC13::m_pDomainObject1");
		delete m_pDomainObject2;
		BaseObject::s_pMemory->Show("delete PTC13::m_pDomainObject2");
	}
	void Initialize() {
	}
	void Finalize() {
	}

	void Run() {
		// test case
		m_pDomainObject1->Run();
		m_pDomainObject2->Run();
	}

};

