#include <22TestApplication/PTS1/PTC14/PTC14.h>
#include <22TestApplication/PTS1/PTC14/Config.h>

PTC14::PTC14(unsigned nClassId, const char* pcClassName)
    : TestCase(nClassId, pcClassName)
    , m_pDomainObject1(nullptr)
    , m_pDomainObject2(nullptr)
{
}

PTC14::~PTC14() {
}

void PTC14::Initialize() {
    m_pDomainObject1 = new("PTC14::DomainObject1") DomainObject14();
    AssertNotNull(m_pDomainObject1);

    m_pDomainObject2 = new("PTC14::DomainObject2") DomainObject14();
    AssertNotNull(m_pDomainObject2);
}

void PTC14::Finalize() {
    delete m_pDomainObject1;
    delete m_pDomainObject2;
}

void PTC14::Run() {
    // test case
    m_pDomainObject1->Run();
    m_pDomainObject2->Run();
}