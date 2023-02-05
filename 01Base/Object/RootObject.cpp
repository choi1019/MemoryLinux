#include <01Base/Object/RootObject.h>
#include <01Base/Aspect/Log.h>
#include <01Base/Aspect/Exception.h>

unsigned RootObject::s_uObjectCount = 0;
IMemory* RootObject::s_pMemory = nullptr;

void* RootObject::operator new (size_t szThis, const char* sMessage) {
    void* pAllocated = RootObject::s_pMemory->SafeMalloc(szThis, sMessage);
    return pAllocated;
}
void* RootObject::operator new[] (size_t szThis, const char* sMessage) {
        return RootObject::operator new(szThis, sMessage);
}
void RootObject::operator delete(void* pObject) {
     RootObject::s_pMemory->SafeFree(pObject);
}
void RootObject::operator delete[](void* pObject) {
    RootObject::s_pMemory->SafeFree(pObject);
}

// dummy
void RootObject::operator delete(void* pObject, const char* sMessage) {
    throw Exception((unsigned)IMemory::EException::_eNotSupport, "RootObject::delete", (size_t)pObject);
}
void RootObject::operator delete[](void* pObject, const char* sMessage) {
    throw Exception((unsigned)IMemory::EException::_eNotSupport, "RootObject::delete[]", (size_t)pObject);
}
