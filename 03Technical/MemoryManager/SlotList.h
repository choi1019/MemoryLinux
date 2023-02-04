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
	static SlotList* s_pSlotListFree;

	void* operator new(size_t szThis, const char* sMessage);
	void operator delete(void* pObject);
	void operator delete(void* pObject, const char* sMessage);

private:
	size_t m_idxPage;
	size_t m_szSlot;
	size_t m_numSlots;
	size_t m_numMaxSlots;
	Slot* m_pSlotHead;

	bool m_bGarbage;

	SlotList* m_pNext;
	SlotList* m_pSibling;

	PageIndex* m_pPageIndex;
	PageList* m_pPageList;
	
public:
	// getters and setters
	size_t GetIdxPage() { return this->m_idxPage; }
	size_t GetSzSlot() { return this->m_szSlot; }
	size_t GetNumSlots() { return this->m_numSlots; }
	bool IsGarbage() { return this->m_bGarbage; }

	SlotList* GetPNext() { return this->m_pNext; }
	void SetPNext(SlotList* pNext) { this->m_pNext = pNext; }
	SlotList* GetPSibling() { return this->m_pSibling; }
	void SetPSibling(SlotList* pSibling) { this->m_pSibling = pSibling; }
	PageIndex* GetPPageIndex() { return this->m_pPageIndex; }

public:
	SlotList(
		size_t szSlot, 
		int nClassId = _SlotList_Id,
		const char* pClassName = _SlotList_Name)
		: MemoryObject(nClassId, pClassName)
		
		, m_szSlot(szSlot)
		, m_pPageList(nullptr)

		, m_idxPage(0)
		, m_numSlots(0)
		, m_numMaxSlots(0)
		, m_pSlotHead(nullptr)
		, m_bGarbage(false)

		, m_pNext(nullptr)
		, m_pSibling(nullptr)
		, m_pPageIndex(nullptr)

	{
		LOG_HEADER("SlotList::SlotList Dummy(m_szSlote)", m_szSlot);
		LOG_FOOTER("SlotList::SlotList Dummy");
	}
	SlotList(
		size_t szSlot, 
		PageList* pPageList,
		int nClassId = _SlotList_Id,
		const char* pClassName = _SlotList_Name)
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
		size_t numPagesRequired = m_szSlot / szPage;
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

		LOG_HEADER("SlotList::SlotList(idxPage, m_szSlot, numMaxSlots, pPage)"
										, m_idxPage, m_szSlot, m_numMaxSlots, (size_t)pPage);
		// generate slots
		this->m_pSlotHead = (Slot*)pPage;
		Slot* pSlot = this->m_pSlotHead;
		Slot* pPreviousSlot = pSlot;
		for (int i = 0; i < m_numSlots; i++) {
			LOG_NEWLINE("Slot-", (size_t)pSlot);
			pSlot->pNext = (Slot*)((size_t)pSlot + m_szSlot);
			pPreviousSlot = pSlot;
			pSlot = pSlot->pNext;
		}
		pPreviousSlot->pNext = nullptr;
		LOG_FOOTER("SlotList::SlotList");
	}
	virtual ~SlotList() {
		this->m_pPageList->FreePages(this->m_idxPage);
	}
	virtual void Initialize() {
		MemoryObject::Initialize();
	}
	virtual void Finalize() {
		MemoryObject::Finalize();
	}

	Slot* AllocateSlot() {
		LOG_HEADER("SlotList::PopSlot()");
		Slot* pSlot = this->m_pSlotHead;
		this->m_pSlotHead = this->m_pSlotHead->pNext;
		this->m_numSlots--;
		LOG_FOOTER("SlotList::PopSlot(pSlot, m_numSlots)", (size_t)pSlot, m_numSlots);
		return pSlot;
	}
	void FreeSlot(Slot* pSlotFree) {
		LOG_HEADER("SlotList::FreeSlot(pSlotFree, idxPage)", (size_t)pSlotFree);
		// insert pSlotFree to Slot LIst
		pSlotFree->pNext = m_pSlotHead;
		m_pSlotHead = pSlotFree;
		this->m_numSlots++;
		if (m_numSlots == m_numMaxSlots) {
			// this is garbage
			LOG_FOOTER("SlotList::FreeSlot->Garbage");
			this->m_bGarbage = true;
		}
		LOG_FOOTER("SlotList::FreeSlot");
		this->m_bGarbage = false;
	}

	
	// maintenance
	virtual void Show(const char* pTitle) {
		LOG_HEADER("SlotList::Show(m_szSlot,Index)", pTitle, m_szSlot, (size_t)GetPPageIndex()->GetIndex());
		Slot* pSlot = this->m_pSlotHead;
		while (pSlot != nullptr) {
			LOG_NEWLINE("Slot-", (size_t)pSlot);
			pSlot = pSlot->pNext;
		}
		LOG_FOOTER("SlotList::Show(m_numMaxSlots, m_numSlots)", pTitle, m_numMaxSlots, m_numSlots);
	};
};

