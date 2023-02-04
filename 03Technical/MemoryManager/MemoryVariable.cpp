#include <03Technical/MemoryManager/MemoryVariable.h>
#include <01Base/Aspect/Exception.h>
#include <01Base/Aspect/Log.h>
#include <stdlib.h>

void* MemoryVariable::s_pMemoryAllocated = nullptr;
size_t MemoryVariable::s_szMemoryAllocated = 0;
void* MemoryVariable::s_pMemoryCurrent = nullptr;
size_t MemoryVariable::s_szMemoryCurrent = 0;

void* MemoryVariable::operator new(size_t szThis, void* pMemoryAllocated, size_t szMemoryllocated) {
    if (szMemoryllocated < szThis) {
        throw Exception((unsigned)IMemory::EException::_eNoMoreSystemMemory, "new MemoryVariable", "_eNoMoreSystemMemory");
    }
    MemoryVariable::s_pMemoryAllocated = pMemoryAllocated;
    MemoryVariable::s_szMemoryAllocated = szMemoryllocated;

    void* pMemoryVariable = RootObject::s_pMemory->SafeMalloc(szThis, "");
    LOG_NEWLINE("@new MemoryVariable(szThis, pMemoryAllocated, szMemoryllocated)", szThis, (size_t)pMemoryAllocated, szMemoryllocated);
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
    LOG_HEADER("MemoryVariable::MemoryVariable(pMemeory,szMemory,szPage,szSlotUnit)"
        , (size_t)s_pMemoryAllocated, s_szMemoryAllocated, szPage, szSlotUnit);
    this->m_pPageList = new("PageList") PageList((size_t)s_pMemoryAllocated, s_szMemoryAllocated, m_szPage);
    this->m_pHead = nullptr;
    this->m_pFreeHead = nullptr;
    this->m_szUnitExponentOf2 = (size_t)(log2(static_cast<double>(this->m_szUnit)));
    this->m_szPageExponentOf2 = (size_t)(log2(static_cast<double>(this->m_szPage)));

    // set as an application memory manager
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
    size_t szSlot = szObject;

    // multiple of WORD
    szSlot >>= m_szUnitExponentOf2;
    szSlot <<= m_szUnitExponentOf2;
    if (szSlot < szObject) {
        szSlot += m_szUnit;
    }

    LOG_HEADER("Memory::Malloc(szObject, szSlot)", szObject, szSlot);
    if (m_pHead == nullptr) {
        // if any SlotList is not generated
        LOG_NEWLINE("if (m_pHead == nullptr)");
        m_pHead = new("SlotList") SlotList(szSlot, m_pPageList);
    }
    else {
        LOG_NEWLINE("else (m_pHead != nullptr)");
    }

    Slot* pSlot = m_pHead->Malloc(szSlot, nullptr);
    LOG_FOOTER("Memory::Malloc(pSlot)", (size_t)pSlot);

    return pSlot;
}

void MemoryVariable::Free(void* pObject) {
    size_t indexPage = ((size_t)pObject - (size_t)s_pMemoryAllocated) / m_szPage;

    LOG_HEADER("Memory::Free(pObject, indexPage)", (size_t)pObject, indexPage);
    bool found = this->m_pHead->Free((Slot*)pObject, indexPage);
    // if m_pHead is a target SlotList
    if (found) {
        LOG_NEWLINE("if(found)");
        // if m_pHead is a Garbage
        if (this->m_pHead->IsGarbage()) {
            LOG_NEWLINE("if (this->m_pHead->IsGarbage())");
            SlotList* pGarbage = this->m_pHead;
            if (this->m_pHead->GetPSibling() != nullptr) {
                LOG_NEWLINE("if (this->m_pHead->GetPSibling() != nullptr)");
                // promote pSibling and delete pGarbage
                this->m_pHead = pGarbage->GetPSibling();
                this->m_pHead->SetPNext(pGarbage->GetPNext());
            }
            else {
                LOG_NEWLINE("if (this->m_pHead->GetPSibling() == nullptr)");
                this->m_pHead = pGarbage->GetPNext();
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
    LOG_HEADER("MemoryVariable::Show-", pTitle);
    // LOG_NEWLINE("SystemMemory(size, current, allocated)"
    // 	, MemoryObject::s_szSystemMemoryAllocated
    // 	, (size_t)MemoryObject::s_pCurrentSystemMemoryAllocated
    // 	, (size_t)MemoryObject::s_pSystemMemoryAllocated
    // );
    m_pPageList->Show("");

    SlotList* pSlotList = this->m_pHead;
    while (pSlotList != nullptr) {
        pSlotList->Show("Next");
        SlotList* pSiblingSlotList = pSlotList->GetPSibling();
        while (pSiblingSlotList != nullptr) {
            pSiblingSlotList->Show("Sibling");
            pSiblingSlotList = pSiblingSlotList->GetPSibling();
        }
        pSlotList = pSlotList->GetPNext();
    }
    LOG_FOOTER("Memory::Show");
};