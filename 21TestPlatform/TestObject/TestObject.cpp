#include <21TestPlatform/TestObject/TestObject.h>
#include <21TestPlatform/TestAspect/TestLog.h>
#include <21TestPlatform/TestAspect/TestException.h>

void* TestObject::operator new(size_t szThis, const char* sMessage) {
    void* pObject = malloc(szThis);
 //   TESTLOG_NEWLINE("#TestObject::malloc (sMessage, szThis, pObject)", sMessage, szThis, (size_t)pObject);
    return pObject;
}
void TestObject::operator delete(void* pObject) {
 //   TESTLOG_NEWLINE("#TestObject::free (pObject)", (size_t)pObject);
    free(pObject);
}
void TestObject::operator delete(void* pObject, const char* sMessage) {
    throw TestException(1, "TestObject", "delete", "not support");
}

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
    TESTLOG_HEADER(this->GetClassName(), "Initialize", "Start");
}

void TestObject::Initialize() {
}

void TestObject::BeforeRun() {
    TESTLOG_FOOTER(this->GetClassName(), "Initialize", "End");
    TESTLOG_HEADER(this->GetClassName(), "Run", "Start");
}

void TestObject::Run() {
}

void TestObject::AfterRun() {
    TESTLOG_FOOTER(this->GetClassName(), "Run", "End");
    TESTLOG_HEADER(this->GetClassName(), "Finalize", "Start");
}

void TestObject::Finalize() {
}

void TestObject::AfterFinalize() {
    TESTLOG_FOOTER(this->GetClassName(), "Finalize", "End")
}