#include <03Technical/MemoryManager/MemoryEven.h>
#include <01Base/Aspect/Exception.h>
#include <01Base/Aspect/Log.h>

void* MemoryEven::operator new(size_t szThis, const char* sMessage) {
    void* pMemoryEven = MemoryObject::s_pMemory->SafeMalloc(szThis, sMessage);    
    return pMemoryEven;
}
void MemoryEven::operator delete(void* pObject) {
    MemoryObject::s_pMemory->SafeFree(pObject);
 }
void MemoryEven::operator delete(void* pObject, const char* sMessage) {
    throw Exception((unsigned)IMemory::EException::_eNotSupport, "delete MemoryEven", "_eNotSupport");
}


// constructors and destructors
MemoryEven::MemoryEven(unsigned szSlot, PageList* pPageList, int nClassId, const char* pClassName)
    : MemoryObject(nClassId, pClassName)
    , m_szSlot(szSlot)
    , m_pPageList(pPageList)
{
    this->m_pSlotList = new("MemoryEven::newSlotList1") SlotList(m_szSlot, m_pPageList);
}
MemoryEven::~MemoryEven() {
}

void MemoryEven::Initialize() {
    MemoryObject::Initialize();
}
void MemoryEven::Finalize() {
    MemoryObject::Finalize();
}

// methods
void* MemoryEven::Malloc(const char* sMessage) {
    SlotList *pCurrent = m_pSlotList;
    while (pCurrent != nullptr) {
        if (pCurrent->GetNumSlots() > 0) {
            return pCurrent->AllocateSlot();
        }
        pCurrent = pCurrent->GetPNext();
    }
    SlotList *pNewSlotList = new("MemoryEven::newSlotList2") SlotList(m_szSlot, m_pPageList);
    Slot* pAllocated = pNewSlotList->AllocateSlot();

    pNewSlotList->SetPNext(m_pSlotList);
    m_pSlotList = pNewSlotList;

    return pAllocated;
}

void MemoryEven::Free(void* pObject) {
//    int idxPage = ((size_t)pObject - (size_t)s_pAllocated) / m_szPage;
    SlotList *pPrevious = m_pSlotList;
    SlotList *pCurrent = pPrevious->GetPNext();
    while (pCurrent != nullptr) {
        if (pCurrent->GetNumSlots() > 0) {
 //           pCurrent->FreeSlot(pObject);
        }
        pCurrent = pCurrent->GetPNext();
    }
    throw Exception((unsigned)IMemory::EException::_eSlotlistFreeFailed, "MemoryEven", "Free", (size_t)pObject);
}

void* MemoryEven::SafeMalloc(size_t szObject, const char* sMessage)
{
    Lock();
    void* pMemoryAllocated = this->Malloc(sMessage);
    UnLock();
    return pMemoryAllocated;
}
void MemoryEven::SafeFree(void* pObject) {
    Lock();
    this->Free(pObject);
    UnLock();
}

// maintenance
void MemoryEven::Show(const char* pTitle) {
    LOG_HEADER("MemoryEven::Show-");
    m_pPageList->Show("");
    SlotList* pSlotList = this->m_pSlotList;
    while (pSlotList != nullptr) {
        pSlotList->Show("Next");
        pSlotList = pSlotList->GetPNext();
    }
    LOG_FOOTER("MemoryEven::Show");
};