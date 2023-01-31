#include <22TestApplication/PTS2/PTS2.h>

#include <22TestApplication/PTS1/Config.h>
#include <22TestApplication/PTS2/PTC21/PTC21.h>
#include <22TestApplication/PTS2/PTC22/PTC22.h>

PTS2::PTS2(unsigned nClassId, const char* pClassName)
    : TestSuite(nClassId, pClassName)
{
}

PTS2::~PTS2() {
}

void PTS2::Initialize() {
    this->add(new("PTC21") PTC21());
    this->add(new("PTC22") PTC22());
}

void PTS2::Finalize() {        
}