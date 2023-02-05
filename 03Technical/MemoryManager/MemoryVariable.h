#pragma once

#include <03Technical/typedef.h>
#define _MemoryVariable_Id _GET_CLASS_UID(_ELayer_Technical::_eMemoryVariable)
#define _MemoryVariable_Name "MemoryVariable"

#include <01Base/Memory/IMemory.h>
#include <03Technical/MemoryManager/MemoryObject.h>

#include <03Technical/MemoryManager/PageList.h>
#include <03Technical/MemoryManager/SlotList.h>

class MemoryVariable :public MemoryObject, public IMemory
{
public:
	// memory for user applicationh
	static void* s_pAllocated;
	static size_t s_szAllocated;
	static void* s_pCurrent;
	static size_t s_szCurrent;

	void* operator new(size_t szThis, void* pMemoryAllocated, size_t szMemoryllocated);
	void operator delete(void* pObject);
	void operator delete(void* pObject, void* pMemoryAllocated, size_t szMemoryllocated);

private:
	// attributes
	unsigned m_szUnit;
	unsigned m_szPage;

	SlotList* m_pSlotListHead;
	PageList* m_pPageList;

	unsigned m_szUnitExponentOf2;

protected:
	// critical section
	virtual void Lock() = 0;
	virtual void UnLock() = 0;

	void* Malloc(size_t szObject, const char* pcName = "");
	void Free(void* pObject);

public:
	// constructors and destructors
	MemoryVariable(
		unsigned szPage,
		unsigned szSlotUnit,
		int nClassId = _MemoryVariable_Id,
		const char* pClassName = _MemoryVariable_Name);
	virtual ~MemoryVariable();;

	virtual void Initialize();
	virtual void Finalize();

	// methods
	void* SafeMalloc(size_t szAllocate, const char* pcName = "") override;
	void SafeFree(void* pObject) override;

	// maintenance
	void Show(const char* pTitle) override;
};
