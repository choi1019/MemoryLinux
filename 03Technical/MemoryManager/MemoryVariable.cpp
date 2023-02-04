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
    MemoryVariable::s_pAllocated = pMemoryAllocated;
    MemoryVariable::s_szAllocated = szMemoryllocated;
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
    this->m_pSlotListHead = nullptr;
    this->m_szUnitExponentOf2 = (size_t)(log2(static_cast<double>(this->m_szUnit)));
    this->m_szPageExponentOf2 = (size_t)(log2(static_cast<double>(this->m_szPage)));

    // set memory manager of BaseObject as this
    BaseObject::s_pMemory = this;

    LOG_FOOTER("MemoryVariable::MemoryVariable");
}
MemoryVariable::~MemoryVariable() 
{
    delete this->m_pPageList;
}

void MemoryVariable::Initialize() {
    LOG_HEADER("MemoryVariable::Initialize");
    MemoryObject::Initialize();
    this->m_pPageList->Initialize();
    LOG_FOOTER("MemoryVariable::Initialize");
}
void MemoryVariable::Finalize() {
    LOG_HEADER("MemoryVariable::Finalize");
    MemoryObject::Finalize();
    this->m_pPageList->Finalize();
    LOG_FOOTER("MemoryVariable::Finalize");
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
    if (m_pSlotListHead == nullptr) {
        // if any SlotList is not generated
        LOG_NEWLINE("if (m_pSlotListHead == nullptr)");
        m_pSlotListHead = new("SlotList") SlotList(szSlot, m_pPageList);
    }
    else {
        LOG_NEWLINE("else (m_pSlotListHead != nullptr)");
    }

    Slot* pSlot = m_pSlotListHead->Malloc(szSlot, nullptr);
    LOG_FOOTER("MemoryVariable::Malloc(pSlot)", (size_t)pSlot);

    return pSlot;
}

void MemoryVariable::Free(void* pObject) {
    size_t idxPage = ((size_t)pObject - (size_t)s_pAllocated) / m_szPage;

    LOG_HEADER("Memory::Free(pObject, idxPage)", (size_t)pObject, idxPage);
    bool found = this->m_pSlotListHead->Free((Slot*)pObject, idxPage);
    // if m_pSlotListHead is a target SlotList
    if (found) {
        LOG_NEWLINE("if(found)");
        // if m_pSlotListHead is a Garbage
        if (this->m_pSlotListHead->IsGarbage()) {
            LOG_NEWLINE("if (this->m_pSlotListHead->IsGarbage())");
            SlotList* pGarbage = this->m_pSlotListHead;
            if (this->m_pSlotListHead->GetPSibling() != nullptr) {
                LOG_NEWLINE("if (this->m_pSlotListHead->GetPSibling() != nullptr)");
                // promote pSibling and delete pGarbage
                this->m_pSlotListHead = pGarbage->GetPSibling();
                this->m_pSlotListHead->SetPNext(pGarbage->GetPNext());
            }
            else {
                LOG_NEWLINE("if (this->m_pSlotListHead->GetPSibling() == nullptr)");
                this->m_pSlotListHead = pGarbage->GetPNext();
            }
            LOG_NEWLINE("delete pGarbage", (size_t)pGarbage);
            delete pGarbage;
        }
    }
    else {
        throw Exception((unsigned)IMemory::EException::_ePageIndexNotFound, "Memory", "Free", (size_t)pObject);
    }
    LOG_FOOTER("Memory::Free");
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