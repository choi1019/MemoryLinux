#pragma once

#include <03Technical/typedef.h>
#define _SlotList_Id _GET_CLASS_UID(_ELayer_Technical::_eSlotList)
#define _SlotList_Name "SlotList"

#include <03Technical/MemoryManager/MemoryObject.h>
#include <03Technical/MemoryManager/PageList.h>

class Slot {
public:
	Slot* pNext;
};

class SlotList : public MemoryObject {
public:
	static SlotList* s_pSlotListRecycle;

	void* operator new(size_t szThis, const char* sMessage);
	void operator delete(void* pObject);
	void operator delete(void* pObject, const char* sMessage);

private:
	int m_idxPage;
	size_t m_szSlot;
	unsigned m_numSlots;
	unsigned m_numMaxSlots;
	Slot* m_pSlotHead;

	bool m_bGarbage;

	SlotList* m_pNext;
	SlotList* m_pSibling;

	PageIndex* m_pPageIndex;
	PageList* m_pPageList;
	
public:
	// getters and setters
	int GetIdxPage() { return this->m_idxPage; }
	size_t GetSzSlot() { return this->m_szSlot; }
	unsigned GetNumSlots() { return this->m_numSlots; }
	bool IsGarbage() { return this->m_bGarbage; }

	SlotList* GetPNext() { return this->m_pNext; }
	void SetPNext(SlotList* pNext) { this->m_pNext = pNext; }
	SlotList* GetPSibling() { return this->m_pSibling; }
	void SetPSibling(SlotList* pSibling) { this->m_pSibling = pSibling; }
//	PageIndex* GetPPageIndex() { return this->m_pPageIndex; }

public:
	// for head SlotList
	SlotList(size_t szSlot, 
		int nClassId = _SlotList_Id,
		const char* pClassName = _SlotList_Name);
	// for normal SlotList
	SlotList(size_t szSlot, PageList* pPageList,
		int nClassId = _SlotList_Id,
		const char* pClassName = _SlotList_Name);
	virtual ~SlotList();
	virtual void Initialize();
	virtual void Finalize();

	Slot* AllocateSlot();
	void FreeSlot(Slot* pSlotFree);	
	// maintenance
	virtual void Show(const char* pTitle);
};

