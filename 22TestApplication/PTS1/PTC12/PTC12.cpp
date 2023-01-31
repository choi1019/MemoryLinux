	#include <22TestApplication/PTS1/
    PTC12(
		int nClassId = _PTC12_Id,
		const char* pcClassName = _PTC12_Name)
		: TestCase(nClassId, pcClassName)
		, m_pDomainObject1(nullptr)
		, m_pDomainObject2(nullptr)
	{
		m_pDomainObject1 = new("PTC12::DomainObject1") DomainObject12();
		BaseObject::s_pMemory->Show("");
		m_pDomainObject2 = new("PTC12::DomainObject2") DomainObject12();
		BaseObject::s_pMemory->Show("");
	}
	virtual ~PTC12() {
		delete m_pDomainObject1;
		BaseObject::s_pMemory->Show("delete PTC12::m_pDomainObject1");
		delete m_pDomainObject2;
		BaseObject::s_pMemory->Show("delete PTC12::m_pDomainObject2");
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