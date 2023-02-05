#include <22TestApplication/PTS1/PTC13/PTC13.h>
#include <22TestApplication/PTS1/PTC13/Config.h>

PTC13::PTC13(unsigned nClassId, const char* pcClassName)
    : TestCase(nClassId, pcClassName)
    , m_pDomainObject1(nullptr)
    , m_pDomainObject2(nullptr)
{
}

PTC13::~PTC13() {
}

void PTC13::Initialize() {
    m_pDomainObject1 = new("PTC13::DomainObject1") DomainObject13();
    m_pDomainObject2 = new("PTC13::DomainObject2") DomainObject13();
}

void PTC13::Finalize() {
    delete m_pDomainObject1;
    delete m_pDomainObject2;
}

void PTC13::Run() {
    // test case
    m_pDomainObject1->Run();
    m_pDomainObject2->Run();
}
