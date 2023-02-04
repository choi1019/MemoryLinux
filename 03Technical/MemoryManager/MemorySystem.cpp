#include <03Technical/MemoryManager/MemorySystem.h>
#include <01Base/Aspect/Exception.h>
#include <01Base/Aspect/Log.h>
#include <stdlib.h>

void* MemorySystem::s_pMemoryAllocated = nullptr;
size_t MemorySystem::s_szMemoryAllocated = 0;
void* MemorySystem::s_pMemoryCurrent = nullptr;
size_t MemorySystem::s_szMemoryCurrent = 0;

void* MemorySystem::operator new(size_t szThis, void* pMemoryAllocated, size_t szMemoryllocated) {
    if (szMemoryllocated < szThis) {
        throw Exception((unsigned)IMemory::EException::_eNoMoreSystemMemory, "new MemorySystem", "_eNoMoreSystemMemory");
    }
    MemorySystem::s_pMemoryAllocated = pMemoryAllocated;
    MemorySystem::s_szMemoryAllocated = szMemoryllocated;

    MemorySystem::s_pMemoryCurrent = (void*)((size_t)MemorySystem::s_pMemoryAllocated + szThis);
    MemorySystem::s_szMemoryCurrent = MemorySystem::s_szMemoryAllocated - szThis;
    LOG_NEWLINE("@new MemorySystem(szThis, pMemoryAllocated, szMemoryllocated)", szThis, (size_t)pMemoryAllocated, szMemoryllocated);
    return MemorySystem::s_pMemoryAllocated;
}
void MemorySystem::operator delete(void* pObject) {
    LOG_NEWLINE("@delete SystemMemorySystem(pObject)", (size_t)pObject);
}
void MemorySystem::operator delete(void* pObject, void* pMemoryAllocated, size_t szMemoryllocated) {
    LOG_NEWLINE("@delete SystemMemorySystem(pObject)", (size_t)pObject);
}

void* MemorySystem::Malloc(size_t szObject, const char* sMessage) {
    if (MemorySystem::s_szMemoryCurrent < szObject) {
        throw Exception((unsigned)IMemory::EException::_eNoMoreSystemMemory, sMessage, "new", "_eNoMoreSystemMemory");
    }
    MemorySystem::s_szMemoryCurrent -= szObject;
    void* pMemoryAllocated = MemorySystem::s_pMemoryCurrent;
    MemorySystem::s_pMemoryCurrent = (void*)((size_t)MemorySystem::s_pMemoryCurrent + szObject);
    LOG_NEWLINE("@new ", sMessage, "(szObject, pAllocated)", szObject, (size_t)pMemoryAllocated);
    return pMemoryAllocated;
}
void MemorySystem::Free(void* pObject) {
}

MemorySystem::MemorySystem(int nClassId, const char* pClassName)
    : RootObject(nClassId, pClassName)
{
    RootObject::s_pMemory = this;
}
MemorySystem::~MemorySystem() {
}
void MemorySystem::Initialize() {
}
void MemorySystem::Finalize() {
}

    // methods
void* MemorySystem::SafeMalloc(size_t szAllocate, const char* pcName)
{
    try {
        Lock();
        void* pMemoryAllocated = this->Malloc(szAllocate, pcName);
        UnLock();
        return pMemoryAllocated;
    }
    catch (Exception& exception) {
        exception.Println();
        exit(1);
    }
}
void MemorySystem::SafeFree(void* pObject) {
    try {
        Lock();
        this->Free(pObject);
        UnLock();
    }
    catch (Exception& exception) {
        exception.Println();
        exit(1);
    }
}

// maintenance
void MemorySystem::Show(const char* pTitle) {
}