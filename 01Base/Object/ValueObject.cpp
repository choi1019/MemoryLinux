#include <01Base/Object/ValueObject.h>
#include <01Base/Aspect/Exception.h>
#include <01Base/Aspect/Log.h>

IMemory* ValueObject::s_pMemory = nullptr;

void* ValueObject::operator new (size_t szThis, const char* sMessage) {
    void* pAllocated = ValueObject::s_pMemory->SafeMalloc(szThis, sMessage);
    return pAllocated;
}
void* ValueObject::operator new[] (size_t szThis, const char* sMessage) {
    return ValueObject::operator new(szThis, sMessage);
}
void ValueObject::operator delete(void* pObject) {
    ValueObject::s_pMemory->SafeFree(pObject);
}
void ValueObject::operator delete[](void* pObject) {
    ValueObject::s_pMemory->SafeFree(pObject);
}

// dummy
void ValueObject::operator delete(void* pObject, const char* sMessage) {
     throw Exception((unsigned)IMemory::EException::_eNotSupport, "ValueObject::delete[]", (size_t)pObject);
}
void ValueObject::operator delete[](void* pObject, const char* sMessage) {
     throw Exception((unsigned)IMemory::EException::_eNotSupport, "ValueObject::delete[]", (size_t)pObject);
}
