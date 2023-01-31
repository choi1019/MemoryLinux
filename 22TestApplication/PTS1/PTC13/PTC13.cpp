#include <22TestApplication/PTS1/PTC13/PTC13.h>
#include <22TestApplication/PTS1/PTC13/Config.h>

PTC13::PTC13(unsigned nClassId, const char* pcClassName)
    : TestCase(nClassId, pcClassName)
    , m_pDomainObject1(nullptr)
    , m_pDomainObject2(nullptr)
{
    m_pDomainObject1 = new("PTC13::DomainObject1") DomainObject13();
    BaseObject::s_pMemory->Show("new PTC13::DomainObject1");
    m_pDomainObject2 = new("PTC13::DomainObject2") DomainObject13();
    BaseObject::s_pMemory->Show("new PTC13::DomainObject2");
}

PTC13::~PTC13() {
    delete m_pDomainObject1;
    BaseObject::s_pMemory->Show("delete PTC13::m_pDomainObject1");
    delete m_pDomainObject2;
    BaseObject::s_pMemory->Show("delete PTC13::m_pDomainObject2");
}

void PTC13::Initialize() {
}

void PTC13::Finalize() {
}

void PTC13::Run() {
    // test case
    m_pDomainObject1->Run();
    m_pDomainObject2->Run();
}
