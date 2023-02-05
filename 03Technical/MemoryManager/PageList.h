#pragma once

#include <03Technical/typedef.h>
#define _PageList_Id _GET_CLASS_UID(_ELayer_Technical::_ePageList)
#define _PageList_Name "PageList"

#include <03Technical/MemoryManager/MemoryObject.h>
#include <03Technical/MemoryManager/PageIndex.h>

class PageList : public MemoryObject {
private:
	size_t m_szPage;
	size_t m_numPagesMax;
	size_t m_numPagesAvaiable;

	// pageIndex
	PageIndex** m_apPageIndices;
	
public:
	size_t GetSzPage() { return this->m_szPage; }
	size_t GetNumPagesAvailable() { return this->m_numPagesAvaiable; }
	size_t GetNumPagesMax() { return this->m_numPagesMax; }

public:
	PageList(
		size_t pMemeoryAllocated, 
		size_t szMemoryAllocated, 
		size_t szPage,
		int nClassId = _PageList_Id,
		const char* pClassName = _PageList_Name)
		: MemoryObject(nClassId, pClassName)
		, m_szPage(szPage)
	{
		if (szMemoryAllocated < m_szPage) {
			throw Exception((unsigned)(IMemory::EException::_eMemoryAllocatedIsSmallerThanAPage)
								, "PageList", "PageList", "MemoryTooSmall");
		}
		this->m_numPagesMax = szMemoryAllocated / m_szPage;
		this->m_numPagesAvaiable = this->m_numPagesMax;

		// operator new[] for pointer array
//		this->m_apPageIndices = new("") PageIndex*[m_numPagesMax];
		this->m_apPageIndices = (PageIndex**)(MemoryObject::operator new(sizeof(PageIndex*)*m_numPagesMax, "m_apPageIndices**"));
		for (int index = 0; index < this->m_numPagesMax; index++) {
			m_apPageIndices[index] = new((String("m_apPageIndices-")+index).c_str()) PageIndex(index, pMemeoryAllocated);
			pMemeoryAllocated = pMemeoryAllocated + m_szPage;
		}
	}
	virtual ~PageList() {
	}
	virtual void Initialize() {
	}
	virtual void Finalize() {
	}

	PageIndex* AllocatePages(size_t numPagesRequired) {
		if (m_numPagesAvaiable < numPagesRequired) {
			throw Exception((unsigned)IMemory::EException::_eNoMorePage, "Memory", "Malloc", "_eNoMorePage");
		} else {
			size_t numPagesAllocated = numPagesRequired;
			unsigned indexFound = 0;
			for (unsigned index = 0; index < m_numPagesMax; index++) {
				if ((m_apPageIndices[index]->IsAllocated())) {
					numPagesAllocated = numPagesRequired;
					indexFound = index + 1;
				}
				else {
					numPagesAllocated--;
					if (numPagesAllocated == 0) {
						// found
						m_apPageIndices[indexFound]->SetNumAllocated(numPagesRequired);
						for (int i = 0; i < numPagesRequired; i++) {
							m_apPageIndices[indexFound + i]->SetIsAllocated(true);
						}
						m_numPagesAvaiable = m_numPagesAvaiable - numPagesRequired;
						return m_apPageIndices[indexFound];
					}
				}
			}
			// not found
			// need compaction =====
			throw Exception((unsigned)IMemory::EException::_eNoMorePage, "Memory", "Malloc", "_eNoMorePage");
		}
	}

	void FreePages(size_t indexFree) {
		size_t numPagesAllocated = m_apPageIndices[indexFree]->GetNumAllocated();
		for (size_t i = 0; i < numPagesAllocated; i++) {
			m_apPageIndices[indexFree + i]->SetNumAllocated(1);
			m_apPageIndices[indexFree + i]->SetIsAllocated(false);
		}
		m_numPagesAvaiable = m_numPagesAvaiable + numPagesAllocated;
	}

	void Show(const char* pTitle) {
		LOG_HEADER("PageList::Show(m_numPagesAvaiable)", m_numPagesAvaiable);
		for (int i=0; i< m_numPagesMax; i++) {
			m_apPageIndices[i]->Show("");
		}
		LOG_FOOTER("PageList::Show");
	}
};

