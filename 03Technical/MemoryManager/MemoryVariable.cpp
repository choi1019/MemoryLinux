#include <03Technical/MemoryManager/MemoryVariable.h>
#include <01Base/Aspect/Exception.h>
#include <01Base/Aspect/Log.h>
#include <stdlib.h>
#include <math.h>

void* MemoryVariable::s_pAllocated = nullptr;
size_t MemoryVariable::s_szAllocated = 0;
void* MemoryVariable::s_pCurrent = nullptr;
size_t MemoryVariable::s_szCurrent = 0;

void* MemoryVariable::operator new(size_t szThis, void* pMemoryAllocated, size_t szMemoryllocated) {
    if (szMemoryllocated < szThis) {
        throw Exception((unsigned)IMemory::EException::_eNoMoreSystemMemory, "new MemoryVariable", "_eNoMoreSystemMemory");
    }
    s_pAllocated = pMemoryAllocated;
    s_szAllocated = szMemoryllocated;
    SlotList::s_pSlotListFree = nullptr;

    void* pMemoryVariable = RootObject::s_pMemory->SafeMalloc(szThis, "");
    LOG_NEWLINE("@new MemoryVariable(szThis, pThis, pMemoryAllocated, szMemoryllocated)"
                                    , szThis, pMemoryVariable, (size_t)pMemoryAllocated, szMemoryllocated);
    return pMemoryVariable;
}
void MemoryVariable::operator delete(void* pObject) {
    RootObject::s_pMemory->SafeFree(pObject);
    LOG_NEWLINE("@delete SystemMemoryVariable(pObject)", (size_t)pObject);
}
void MemoryVariable::operator delete(void* pObject, void* pMemoryAllocated, size_t szMemoryllocated) {
    throw Exception((unsigned)IMemory::EException::_eNotSupport, "delete MemoryVariable", "_eNotSupport");
}


// constructors and destructors
MemoryVariable::MemoryVariable(size_t szPage, size_t szSlotUnit, int nClassId, const char* pClassName)
    : m_szPage(szPage)
    , m_szUnit(szSlotUnit)
{
    LOG_HEADER("MemoryVariable::MemoryVariable(pAllocated,szAllocated,szPage,szSlotUnit)"
        , (size_t)s_pAllocated, s_szAllocated, szPage, szSlotUnit);
    this->m_pPageList = new("PageList") PageList((size_t)s_pAllocated, s_szAllocated, m_szPage);
    this->m_pSlotListHead = new("SlotList 0") SlotList(0, m_pPageList);
    this->m_szUnitExponentOf2 = (size_t)(log2(static_cast<double>(this->m_szUnit)));
 
    // set memory manager of BaseObject as this
    BaseObject::s_pMemory = this;

    LOG_FOOTER("MemoryVariable::MemoryVariable");
}
MemoryVariable::~MemoryVariable() 
{
    delete this->m_pPageList;
}

void MemoryVariable::Initialize() {
    MemoryObject::Initialize();
}
void MemoryVariable::Finalize() {
    MemoryObject::Finalize();
}

// methods
void* MemoryVariable::Malloc(size_t szObject, const char* pcName) { 
     // multiple of WORD
    size_t szSlot = szObject;
    szSlot >>= m_szUnitExponentOf2;
    szSlot <<= m_szUnitExponentOf2;
    if (szSlot < szObject) {
        szSlot += m_szUnit;
    }

    LOG_HEADER("MemoryVariable::Malloc(szObject, szSlot)", szObject, szSlot);
     
    SlotList *pPrevious = m_pSlotListHead;
    SlotList *pCurrent = pPrevious->GetPNext(); 
    while (pCurrent != nullptr) {
        // if the same size is found
        if (pCurrent->GetSzSlot() == szSlot) {
            SlotList *pSibling = pCurrent->GetPSibling();
            while (pSibling != nullptr) {
                if (pSibling->GetNumSlots() > 0) {
                    // found
                    Slot* pSlot = pSibling->AllocateSlot();
                    LOG_FOOTER("MemoryVariable::Malloc(pSlot)1", (size_t)pSlot);
                    return pSlot;   
                }
                pSibling = pSibling->GetPNext();
            }
            // if current SlotLists has no more slot
            pSibling = new("SlotList") SlotList(szSlot, m_pPageList);
            Slot* pSlot = pSibling->AllocateSlot();
            // insert a new sibling
            pSibling->SetPSibling(pCurrent->GetPSibling());
            pCurrent->SetPSibling(pSibling);
            LOG_FOOTER("MemoryVariable::Malloc(pSlot)2", (size_t)pSlot);
            return pSlot;  

        } else if (pCurrent->GetSzSlot() > szSlot) {
            // if slotlist is not available
            SlotList *pNewSlotList = new("SlotList") SlotList(szSlot, m_pPageList);
            Slot* pSlot = pNewSlotList->AllocateSlot();
            // insert a new next head
            SlotList *pNext = new("SlotList") SlotList(szSlot);
            pNext->SetPSibling(pNewSlotList);
            pPrevious->SetPNext(pNext);
            pNext->SetPNext(pCurrent);
            LOG_FOOTER("MemoryVariable::Malloc(pSlot)3", (size_t)pSlot);
            return pSlot;  
        }
        pPrevious = pCurrent;
        pCurrent = pCurrent->GetPNext();
    } 
    // if slotlist is null
    SlotList *pNewSlotList = new("SlotList") SlotList(szSlot, m_pPageList);
    Slot* pSlot = pNewSlotList->AllocateSlot();
    // insert a new SlotList
    SlotList *pNext = new("SlotList") SlotList(szSlot);
    m_pSlotListHead->SetPNext(pNext);
    LOG_FOOTER("MemoryVariable::Malloc(pSlot)4", (size_t)pSlot);
    return pSlot;   
}

void MemoryVariable::Free(void* pObject) {
    size_t idxPage = ((size_t)pObject - (size_t)s_pAllocated) / m_szPage;
    LOG_HEADER("MemoryVariable::Free(pObject, idxPage)", (size_t)pObject, idxPage);
    SlotList *pPrevious = m_pSlotListHead;
    SlotList *pCurrent = pPrevious->GetPNext(); 
    while (pCurrent != nullptr) {
        SlotList *pSiblingPrevious = pCurrent;
        SlotList *pSiblingCurrent = pCurrent->GetPNext();
        while(pSiblingCurrent != nullptr) {
            if (pSiblingCurrent->GetIdxPage() == idxPage) {
                // found
                pSiblingCurrent->FreeSlot((Slot *)pObject);
                if (pSiblingCurrent->IsGarbage()) {
                    delete pCurrent;
                }
                LOG_FOOTER("MemoryVariable::Free(pSlot)");
                return;
            }
            pSiblingPrevious = pSiblingCurrent;
            pSiblingCurrent = pSiblingCurrent->GetPSibling();
        }
        pPrevious = pCurrent;
        pCurrent = pCurrent->GetPNext();
    }
    throw Exception((unsigned)IMemory::EException::_ePageIndexNotFound, "MemoryVariable", "Free", (size_t)pObject);
}

void* MemoryVariable::SafeMalloc(size_t szAllocate, const char* pcName)
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
void MemoryVariable::SafeFree(void* pObject) {
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
void MemoryVariable::Show(const char* pTitle) {
    LOG_HEADER("MemoryVariable::Show-", (size_t)s_pCurrent, s_pCurrent);
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
    LOG_FOOTER("MemoryVariable::Show");
};