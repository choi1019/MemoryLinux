#include <01Base/Object/BaseObject.h>
#include <01Base/Aspect/Log.h>


IMemory* BaseObject::s_pMemory = nullptr;

void* BaseObject::operator new (size_t szThis, const char* sMessage) {
    LOG_NEWLINE("@BaseObject::new(sMessage, szThis)", sMessage, szThis);
    void* pAllocated = BaseObject::s_pMemory->SafeMalloc(szThis, sMessage);
    return pAllocated;
}
void* BaseObject::operator new[] (size_t szThis, const char* sMessage) {
        return BaseObject::operator new(szThis, sMessage);
}
void BaseObject::operator delete(void* pObject) {
    LOG_NEWLINE("@BaseObject::delete(pObject)", (size_t)pObject);
    BaseObject::s_pMemory->SafeFree(pObject);
}
void BaseObject::operator delete[](void* pObject) {
    LOG_NEWLINE("@BaseObject::delete[](pObject)", (size_t)pObject);
    BaseObject::s_pMemory->SafeFree(pObject);
}

// dummy
void BaseObject::operator delete(void* pObject, const char* sMessage) {
    LOG_NEWLINE("@DUMMY BaseObject::delete(pObject)", (size_t)pObject);
}
void BaseObject::operator delete[](void* pObject, const char* sMessage) {
    LOG_NEWLINE("@DUMMY BaseObject::delete[](pObject)", (size_t)pObject);
}

BaseObject::BaseObject(int nClassId, const char* pcClassName)
    : RootObject(nClassId, pcClassName)
{
}
BaseObject::~BaseObject() {
}