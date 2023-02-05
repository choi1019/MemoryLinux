#include <03Technical/MemoryManager/MemoryObject.h>
#include <01Base/Aspect/Log.h>
#include <01Base/Aspect/Exception.h>

IMemory* MemoryObject::s_pMemory = nullptr;

void* MemoryObject::operator new (size_t szThis, const char* sMessage) {
    void* pAllocated = MemoryObject::s_pMemory->SafeMalloc(szThis, sMessage);
    return pAllocated;
}
void* MemoryObject::operator new[] (size_t szThis, const char* sMessage) {
        return MemoryObject::operator new(szThis, sMessage);
}
void MemoryObject::operator delete(void* pObject) {
     MemoryObject::s_pMemory->SafeFree(pObject);
}
void MemoryObject::operator delete[](void* pObject) {
    MemoryObject::s_pMemory->SafeFree(pObject);
}

// dummy
void MemoryObject::operator delete(void* pObject, const char* sMessage) {
    throw Exception((unsigned)IMemory::EException::_eNotSupport, "MemoryObject::delete", (size_t)pObject);
}
void MemoryObject::operator delete[](void* pObject, const char* sMessage) {
    throw Exception((unsigned)IMemory::EException::_eNotSupport, "MemoryObject::delete[]", (size_t)pObject);
}