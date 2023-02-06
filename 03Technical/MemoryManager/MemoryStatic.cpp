#include <03Technical/MemoryManager/MemoryStatic.h>
#include <01Base/Aspect/Exception.h>
#include <01Base/Aspect/Log.h>
#include <03Technical/MemoryManager/SlotList.h>

void* MemoryStatic::s_pAllocated = nullptr;
size_t MemoryStatic::s_szAllocated = 0;
void* MemoryStatic::s_pCurrent = nullptr;
size_t MemoryStatic::s_szCurrent = 0;

void* MemoryStatic::operator new(size_t szThis, void* pMemoryAllocated, size_t szMemoryllocated) {
    if (szMemoryllocated < szThis) {
        throw Exception((unsigned)IMemory::EException::_eNoMoreSystemMemory, "new MemoryStatic", "_eNoMoreSystemMemory");
    }
    s_pAllocated = pMemoryAllocated;
    s_szAllocated = szMemoryllocated;
    s_pCurrent = (void*)((size_t)s_pAllocated + szThis);
    s_szCurrent = s_szAllocated - szThis;

    LOG_NEWLINE("MemoryStatic::new", szThis, s_szCurrent, s_szAllocated);
    return s_pAllocated;
}
void MemoryStatic::operator delete(void* pObject) {
}
void MemoryStatic::operator delete(void* pObject, void* pMemoryAllocated, size_t szMemoryllocated) {
    throw Exception((unsigned)IMemory::EException::_eNotSupport, "MemoryStatic::delete", "_eNotSupport");
}

MemoryStatic::MemoryStatic(int nClassId, const char* pClassName)
    : MemoryObject(nClassId, pClassName)
{
    // set memory manager of RootObject as this
    BaseObject::s_pMemory = this;
}
MemoryStatic::~MemoryStatic() {
 }
void MemoryStatic::Initialize() {
    MemoryObject::Initialize();
}
void MemoryStatic::Finalize() {
    MemoryObject::Finalize();
}

// methods
void* MemoryStatic::Malloc(size_t szObject, const char* sMessage) {
    if (s_szCurrent < szObject) {
        throw Exception((unsigned)IMemory::EException::_eNoMoreSystemMemory, sMessage, "MemoryStatic::Malloc", "_eNoMoreSystemMemory");
    }    
    void* pMemoryAllocated = s_pCurrent;
    s_pCurrent = (void*)((size_t)s_pCurrent + szObject);
    s_szCurrent = s_szCurrent - szObject;

    LOG_NEWLINE("MemoryStatic::Malloc(szObject, s_szCurrent, s_szAllocated)", sMessage, szObject, s_szCurrent, s_szAllocated);
    return pMemoryAllocated;
}
void MemoryStatic::Free(void* pObject) {
   LOG_NEWLINE("==> Warning-MemoryStatic::Free(pObject)", (size_t)pObject);
}

void* MemoryStatic::SafeMalloc(size_t szAllocate, const char* sMessage)
{
    Lock();
    void* pMemoryAllocated = this->Malloc(szAllocate, sMessage);
    UnLock();
    return pMemoryAllocated;
}
void MemoryStatic::SafeFree(void* pObject) {
    Lock();
    this->Free(pObject);
    UnLock();
}

// maintenance
void MemoryStatic::Show(const char* pTitle) {
    LOG_HEADER("MemoryStatic::Show-", pTitle);
    LOG_NEWLINE("MemoryStatic(szAllocated, pAllocated, szCurrent, pCurrent)"
    	, s_szAllocated
    	, (size_t)s_pAllocated
        , s_szCurrent
    	, (size_t)s_pCurrent
    );
    LOG_FOOTER("MemoryStatic::Show");
}