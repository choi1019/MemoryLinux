#include <21TestPlatform/TestObject/TestObject.h>
#include <21TestPlatform/TestAspect/TestLog.h>

unsigned TestObject::s_uCounter = 0;

TestObject::TestObject(unsigned nClassId, const char *pcClassName)
    : m_uObjectId(TestObject::s_uCounter++)
	, m_nClassId(nClassId)
	, m_pcClassName(pcClassName)
	, m_szThis(0)
{
}

TestObject::~TestObject() {}


void TestObject::BeforeInitialize() {
    TestLog().PrintSeparator();
    TestLog(this->GetClassName(), "Initialize", "Start").Println();
    TestLog().PrintSeparator();
    TestLog::AddTab();
}

void TestObject::Initialize() {
}

void TestObject::BeforeRun() {
    TestLog().PrintSeparator();
    TestLog(this->GetClassName(), "Run", "Start").Println();
    TestLog().PrintSeparator();
    TestLog::AddTab();
}

void TestObject::Run() {
}

void TestObject::AfterRun() {
    TestLog::RemoveTab();
    TestLog().PrintSeparator();
    TestLog(this->GetClassName(), "Run", "End").Println();
    TestLog().PrintSeparator();
}

void TestObject::Finalize() {
}

void TestObject::AfterFinalize() {
    TestLog::RemoveTab();
    TestLog().PrintSeparator();
    TestLog(this->GetClassName(), "Finalize", "End").Println();
    TestLog().PrintSeparator();
}