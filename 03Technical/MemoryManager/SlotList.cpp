#include <03Technical/MemoryManager/SlotList.h>

SlotList* SlotList::s_pSlotListRecycle = nullptr;

void* SlotList::operator new(size_t szThis, const char* sMessage) {
    void* pNewSlotList = nullptr;
    if (SlotList::s_pSlotListRecycle == nullptr) {
        pNewSlotList = RootObject::s_pMemory->SafeMalloc(szThis, sMessage);
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