#include <03Technical/MemoryManager/MemoryDynamic.h>
#include <01Base/Object/ValueObject.h>
#include <01Base/Aspect/Exception.h>
#include <01Base/Aspect/Log.h>
#include <math.h>

void* MemoryDynamic::s_pAllocated = nullptr;
size_t MemoryDynamic::s_szAllocated = 0;
void* MemoryDynamic::s_pCurrent = nullptr;
size_t MemoryDynamic::s_szCurrent = 0;

void* MemoryDynamic::operator new(size_t szThis, void* pMemoryAllocated, size_t szMemoryllocated) {
    if (szMemoryllocated < szThis) {
        throw Exception((unsigned)IMemory::EException::_eNoMoreSystemMemory, "new MemoryDynamic", "_eNoMoreSystemMemory");
    }
    s_pAllocated = pMemoryAllocated;
    s_szAllocated = szMemoryllocated;

    void* pMemoryDynamic = BaseObject::s_pMemory->SafeMalloc(szThis, "pMemoryDynamic");    
    SlotList::s_pSlotListRecycle = nullptr;

    return pMemoryDynamic;
}
void MemoryDynamic::operator delete(void* pObject) {
    MemoryObject::s_pMemory->SafeFree(pObject);
 }
void MemoryDynamic::operator delete(void* pObject, void* pMemoryAllocated, size_t szMemoryllocated) {
    throw Exception((unsigned)IMemory::EException::_eNotSupport, "delete MemoryDynamic", "_eNotSupport");
}


// constructors and destructors
MemoryDynamic::MemoryDynamic(unsigned szPage, unsigned szSlotUnit, int nClassId, const char* pClassName)
    : MemoryObject(nClassId, pClassName)
    , m_szPage(szPage)
    , m_szUnit(szSlotUnit)
{
    this->m_pPageList = new("PageList") PageList((size_t)s_pAllocated, s_szAllocated, m_szPage);
    this->m_pSlotListHead = new("SlotList 0") SlotList(0);
    this->m_szUnitExponentOf2 = (unsigned)(log2(static_cast<double>(this->m_szUnit)));
 
    // set memory manager of ValueObject as this
    ValueObject::s_pMemory = this;
}
MemoryDynamic::~MemoryDynamic() {
    delete this->m_pPageList;
}

void MemoryDynamic::Initialize() {
    MemoryObject::Initialize();
}
void MemoryDynamic::Finalize() {
    MemoryObject::Finalize();
}

// methods
void* MemoryDynamic::Malloc(size_t szObject, const char* sMessage) { 
     // multiple of WORD
    size_t szSlot = szObject;
    szSlot >>= m_szUnitExponentOf2;
    szSlot <<= m_szUnitExponentOf2;
    if (szSlot < szObject) {
        szSlot += m_szUnit;
    }
    
    LOG_NEWLINE("MemoryDynamic::Malloc(szObject)", sMessage, szObject, s_szCurrent, s_szAllocated);
    SlotList *pPrevious = nullptr;
    SlotList *pCurrent = m_pSlotListHead; 
    while (pCurrent != nullptr) {
        // if the same size is found
        if (pCurrent->GetSzSlot() == szSlot) {
            SlotList *pSibling = pCurrent->GetPSibling();
            while (pSibling != nullptr) {
                if (pSibling->GetNumSlots() > 0) {
                    // found
                    Slot* pSlot = pSibling->AllocateSlot();
                    return pSlot;   
                }
                pSibling = pSibling->GetPNext();
            }
            // if current SlotLists has no more slot
            pSibling = new("SlotList2") SlotList(szSlot, m_pPageList);
            Slot* pSlot = pSibling->AllocateSlot();
            // insert a new sibling
            pSibling->SetPSibling(pCurrent->GetPSibling());
            pCurrent->SetPSibling(pSibling);
            return pSlot;  
        } else if (pCurrent->GetSzSlot() < szSlot) {
            if (pCurrent->GetPNext() == nullptr) {
                // if slotlist is null, add new SlotList to the end
                SlotList *pNewSlotList = new("SlotList3") SlotList(szSlot, m_pPageList);
                Slot* pSlot = pNewSlotList->AllocateSlot();
                // insert a new SlotList
                SlotList *pNext = new("SlotListNext3") SlotList(szSlot);
                pNext->SetPSibling(pNewSlotList);
                pCurrent->SetPNext(pNext);
                pNext->SetPNext(nullptr);
                return pSlot;  
            }
        } else if (pCurrent->GetSzSlot() > szSlot) {
            // if no same size, insert new SlotList
            SlotList *pNewSlotList = new("SlotList4") SlotList(szSlot, m_pPageList);
            Slot* pSlot = pNewSlotList->AllocateSlot();
            // insert a new next head
            SlotList *pNext = new("SlotListNext4") SlotList(szSlot);
            pNext->SetPSibling(pNewSlotList);
            pPrevious->SetPNext(pNext);
            pNext->SetPNext(pCurrent);
            return pSlot;  
        }
        pPrevious = pCurrent;
        pCurrent = pCurrent->GetPNext();
    } 
    throw Exception((unsigned)IMemory::EException::_eSlotlistAllocationFailed
                                        , "MemoryDynamic", "Malloc", "Failed");
}

void MemoryDynamic::Free(void* pObject) {
    LOG_NEWLINE("MemoryDynamic::Free(pObject)", (size_t)pObject);
    
    size_t idxPage = ((size_t)pObject - (size_t)s_pAllocated) / m_szPage;
    SlotList *pCurrent = m_pSlotListHead->GetPNext(); 
    while (pCurrent != nullptr) {
        SlotList *pSiblingPrevious = pCurrent;
        SlotList *pSiblingCurrent = pSiblingPrevious->GetPSibling();
        while(pSiblingCurrent != nullptr) {
            if (pSiblingCurrent->GetIdxPage() == idxPage) {
                // found
                pSiblingCurrent->FreeSlot((Slot *)pObject);
                if (pSiblingCurrent->IsGarbage()) {
                    pSiblingPrevious->SetPSibling(pSiblingCurrent->GetPSibling());
                    LOG_NEWLINE("MemoryDynamic::Free-Garbage", (size_t)pSiblingCurrent);
                    delete pSiblingCurrent;
                }
                return;
            }
            pSiblingPrevious = pSiblingCurrent;
            pSiblingCurrent = pSiblingCurrent->GetPSibling();
        }
        pCurrent = pCurrent->GetPNext();
    }
    throw Exception((unsigned)IMemory::EException::_eSlotlistFreeFailed, "MemoryDynamic", "Free", (size_t)pObject);
}

void* MemoryDynamic::SafeMalloc(size_t szAllocate, const char* pcName)
{
    Lock();
    void* pMemoryAllocated = this->Malloc(szAllocate, pcName);
    UnLock();
    return pMemoryAllocated;
}
void MemoryDynamic::SafeFree(void* pObject) {
    Lock();
    this->Free(pObject);
    UnLock();
}

// maintenance
void MemoryDynamic::Show(const char* pTitle) {
    LOG_HEADER("MemoryDynamic::Show-", (size_t)s_pCurrent, s_pCurrent);
    m_pPageList->Show("");
    SlotList* pSlotList = this->m_pSlotListHead;
    while (pSlotList != nullptr) {
        pSlotList->Show("Next");
        SlotList* pSibling = pSlotList->GetPSibling();
        while (pSibling != nullptr) {
            pSibling->Show("Sibling");
            pSibling = pSibling->GetPSibling();
        }
        pSlotList = pSlotList->GetPNext();
    }
    LOG_FOOTER("MemoryDynamic::Show");
};