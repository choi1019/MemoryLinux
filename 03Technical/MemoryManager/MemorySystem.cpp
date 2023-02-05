#include <03Technical/MemoryManager/MemorySystem.h>
#include <01Base/Aspect/Exception.h>
#include <01Base/Aspect/Log.h>
#include <03Technical/MemoryManager/SlotList.h>

void* MemorySystem::s_pAllocated = nullptr;
size_t MemorySystem::s_szAllocated = 0;
void* MemorySystem::s_pCurrent = nullptr;
size_t MemorySystem::s_szCurrent = 0;

void* MemorySystem::operator new(size_t szThis, void* pMemoryAllocated, size_t szMemoryllocated) {
    if (szMemoryllocated < szThis) {
        throw Exception((unsigned)IMemory::EException::_eNoMoreSystemMemory, "new MemorySystem", "_eNoMoreSystemMemory");
    }
    s_pAllocated = pMemoryAllocated;
    s_szAllocated = szMemoryllocated;
    s_pCurrent = (void*)((size_t)s_pAllocated + szThis);
    s_szCurrent = s_szAllocated - szThis;

    LOG_NEWLINE("MemorySystem::new", szThis, s_szCurrent, s_szAllocated);
    return s_pAllocated;
}
void MemorySystem::operator delete(void* pObject) {
}
void MemorySystem::operator delete(void* pObject, void* pMemoryAllocated, size_t szMemoryllocated) {
    throw Exception((unsigned)IMemory::EException::_eNotSupport, "MemorySystem::delete", "_eNotSupport");
}

void* MemorySystem::Malloc(size_t szObject, const char* sMessage) {
    if (s_szCurrent < szObject) {
        throw Exception((unsigned)IMemory::EException::_eNoMoreSystemMemory, sMessage, "MemorySystem::Malloc", "_eNoMoreSystemMemory");
    }    
    void* pMemoryAllocated = s_pCurrent;
    s_pCurrent = (void*)((size_t)s_pCurrent + szObject);
    s_szCurrent = s_szCurrent - szObject;

    LOG_NEWLINE(sMessage, szObject, s_szCurrent, s_szAllocated);
    return pMemoryAllocated;
}
void MemorySystem::Free(void* pObject) {
}

MemorySystem::MemorySystem(int nClassId, const char* pClassName)
    : MemoryObject(nClassId, pClassName)
{
    // set memory manager of RootObject as this
    RootObject::s_pMemory = this;
}
MemorySystem::~MemorySystem() {
 }
void MemorySystem::Initialize() {
    MemoryObject::Initialize();
}
void MemorySystem::Finalize() {
    MemoryObject::Finalize();
}

    // methods
void* MemorySystem::SafeMalloc(size_t szAllocate, const char* sMessage)
{
    Lock();
    void* pMemoryAllocated = this->Malloc(szAllocate, sMessage);
    UnLock();
    return pMemoryAllocated;

}
void MemorySystem::SafeFree(void* pObject) {
    Lock();
    this->Free(pObject);
    UnLock();
}

// maintenance
void MemorySystem::Show(const char* pTitle) {
    LOG_HEADER("MemorySystem::Show-", pTitle);
    LOG_NEWLINE("MemorySystem(szAllocated, pAllocated, szCurrent, pCurrent)"
    	, s_szAllocated
    	, (size_t)s_pAllocated
        , s_szCurrent
    	, (size_t)s_pCurrent
    );
    LOG_FOOTER("MemorySystem::Show");
}