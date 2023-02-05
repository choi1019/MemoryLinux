#include <01Base/Object/BaseObject.h>
#include <01Base/Aspect/Exception.h>
#include <01Base/Aspect/Log.h>

IMemory* BaseObject::s_pMemory = nullptr;

void* BaseObject::operator new (size_t szThis, const char* sMessage) {
    void* pAllocated = BaseObject::s_pMemory->SafeMalloc(szThis, sMessage);
    return pAllocated;
}
void* BaseObject::operator new[] (size_t szThis, const char* sMessage) {
    return BaseObject::operator new(szThis, sMessage);
}
void BaseObject::operator delete(void* pObject) {
    BaseObject::s_pMemory->SafeFree(pObject);
}
void BaseObject::operator delete[](void* pObject) {
    BaseObject::s_pMemory->SafeFree(pObject);
}

// dummy
void BaseObject::operator delete(void* pObject, const char* sMessage) {
     throw Exception((unsigned)IMemory::EException::_eNotSupport, "BaseObject::delete[]", (size_t)pObject);
}
void BaseObject::operator delete[](void* pObject, const char* sMessage) {
     throw Exception((unsigned)IMemory::EException::_eNotSupport, "BaseObject::delete[]", (size_t)pObject);
}
