#pragma once

#include <03Technical/typedef.h>
#define _PageIndex_Id _GET_CLASS_UID(_ELayer_Technical::_ePageIndex)
#define _PageIndex_Name "PageIndex"

#include <03Technical/MemoryManager/MemoryObject.h>

class Page {
public:
};

class PageIndex : public MemoryObject {
private:
	unsigned m_index;
	Page* m_pPage;

	bool m_bAllocated;
	unsigned m_numAllocated;

public:
	unsigned GetIndex() { return this->m_index; }
	Page* GetPPage() { return this->m_pPage; }
	bool IsAllocated() { return this->m_bAllocated; }
	void SetIsAllocated(bool bAllocated) { this->m_bAllocated = bAllocated; }
	unsigned GetNumAllocated() { return this->m_numAllocated; }
	void SetNumAllocated(size_t numAllocated) { this->m_numAllocated = numAllocated; }

public:
	PageIndex(
		unsigned index,
		size_t pMemoryAllocated,
	 	int nClassId = _PageIndex_Id,
		const char* pClassName = _PageIndex_Name)
		: MemoryObject(nClassId, pClassName)		
		, m_index(index)
		, m_pPage((Page*)pMemoryAllocated)	
		, m_numAllocated(1)
		, m_bAllocated(false)
	{
	}
	virtual ~PageIndex() {}

	virtual void Initialize() {
	}
	virtual void Finalize() {
	}

	void Show(const char* pTitle) {
		LOG_NEWLINE("PageIndex::show(index, pPage, numAllocated, bAllocated)"
							, m_index, (size_t)m_pPage, m_numAllocated, m_bAllocated);
	}
};
