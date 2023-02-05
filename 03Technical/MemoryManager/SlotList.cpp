#include <03Technical/MemoryManager/SlotList.h>

SlotList* SlotList::s_pSlotListRecycle = nullptr;

void* SlotList::operator new(size_t szThis, const char* sMessage) {
    void* pNewSlotList = nullptr;
    if (SlotList::s_pSlotListRecycle == nullptr) {
        pNewSlotList = MemoryObject::s_pMemory->SafeMalloc(szThis, sMessage);
    }
    else {
        pNewSlotList = SlotList::s_pSlotListRecycle;
        SlotList::s_pSlotListRecycle = SlotList::s_pSlotListRecycle->GetPNext();
        LOG_NEWLINE("SlotList::newRecycle(pNewSlotList)", (size_t)pNewSlotList);
    }
    return pNewSlotList;
}

void SlotList::operator delete(void* pObject) {
    SlotList* pSlotList = (SlotList*)pObject;
    pSlotList->SetPNext(SlotList::s_pSlotListRecycle);
    SlotList::s_pSlotListRecycle = pSlotList;
 //   LOG_NEWLINE("SlotList::delete(pObject)", (size_t)pObject);
}

void SlotList::operator delete(void* pObject, const char* sMessage) {
    throw Exception((unsigned)IMemory::EException::_eNotSupport, "SlotList::delete", "_eNotSupport");
}

SlotList::SlotList(size_t szSlot, int nClassId,const char* pClassName)
    : MemoryObject(nClassId, pClassName)
    
    , m_szSlot(szSlot)
    , m_pPageList(nullptr)

    , m_idxPage(-1)
    , m_numSlots(0)
    , m_numMaxSlots(0)
    , m_pSlotHead(nullptr)
    , m_bGarbage(false)

    , m_pNext(nullptr)
    , m_pSibling(nullptr)
    , m_pPageIndex(nullptr)
{
}
SlotList::SlotList(size_t szSlot, PageList* pPageList,int nClassId, const char* pClassName)
    : MemoryObject(nClassId, pClassName)
    
    , m_szSlot(szSlot)
    , m_pPageList(pPageList)

    , m_idxPage(0)
    , m_numSlots(0)
    , m_numMaxSlots(0)
    , m_pSlotHead(nullptr)
    , m_bGarbage(false)

    , m_pNext(nullptr)
    , m_pSibling(nullptr)
    , m_pPageIndex(nullptr)
{
    // compute the number of pages required
    size_t szPage = m_pPageList->GetSzPage();
    // oversized slot bigger than a page
    unsigned numPagesRequired = m_szSlot / szPage;
    if (m_szSlot > numPagesRequired * szPage) {
        numPagesRequired++;
    }

    // get required number of pages
    this->m_pPageIndex = m_pPageList->AllocatePages(numPagesRequired);
    Page* pPage = this->m_pPageIndex->GetPPage();
    this->m_idxPage = this->m_pPageIndex->GetIndex();

    // compute the number of slots allocatable
    this->m_numMaxSlots = numPagesRequired * szPage / m_szSlot;
    this->m_numSlots = this->m_numMaxSlots;

    // generate slots
    this->m_pSlotHead = (Slot*)pPage;
    Slot* pSlot = this->m_pSlotHead;
    Slot* pPreviousSlot = pSlot;
    for (int i = 0; i < m_numSlots; i++) {
        pSlot->pNext = (Slot*)((size_t)pSlot + m_szSlot);
        pPreviousSlot = pSlot;
        pSlot = pSlot->pNext;
    }
    pPreviousSlot->pNext = nullptr;
}
SlotList::~SlotList() {
    this->m_pPageList->FreePages(this->m_idxPage);
}
void SlotList::Initialize() {
    MemoryObject::Initialize();
}
void SlotList::Finalize() {
    MemoryObject::Finalize();
}

Slot* SlotList::AllocateSlot() {
    Slot* pSlot = this->m_pSlotHead;
    this->m_pSlotHead = this->m_pSlotHead->pNext;
    this->m_numSlots--;
    return pSlot;
}
void SlotList::FreeSlot(Slot* pSlotFree) {
    // insert pSlotFree to Slot LIst
    pSlotFree->pNext = m_pSlotHead;
    m_pSlotHead = pSlotFree;
    this->m_numSlots++;
    if (m_numSlots == m_numMaxSlots) {
        // this is garbage
        this->m_bGarbage = true;			
    } else {
        this->m_bGarbage = false;
    }
}

// maintenance
void SlotList::Show(const char* pTitle) {
    LOG_HEADER("SlotList::Show(m_szSlot,Index)", pTitle, m_szSlot, m_idxPage);
    Slot* pSlot = this->m_pSlotHead;
    while (pSlot != nullptr) {
        LOG_NEWLINE("Slot-", (size_t)pSlot);
        pSlot = pSlot->pNext;
    }
    LOG_FOOTER("SlotList::Show(m_numMaxSlots, m_numSlots)", pTitle, m_numMaxSlots, m_numSlots);
};