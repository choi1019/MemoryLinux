
#include <22TestApplication/PTS1/PTC11/PTC11.h>
#include <22TestApplication/PTS1/PTC11/Config.h>

PTC11::PTC11(unsigned nClassId, const char* pcClassName)
    : TestCase(nClassId, pcClassName)
    , m_pDomainObject(nullptr) {
}
PTC11::~PTC11() {
}

void PTC11::Initialize() {
    this->m_pDomainObject = new("PTC11::DomainObject") DomainObject11();
}
void PTC11::Finalize() {
    delete this->m_pDomainObject;
}
void PTC11::Run() {
    // test case
    this->m_pDomainObject->Run();
}