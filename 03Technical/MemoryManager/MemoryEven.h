#pragma once

#include <03Technical/typedef.h>
#define _MemoryEven_Id _GET_CLASS_UID(_ELayer_Technical::_eMemoryEven)
#define _MemoryEven_Name "MemoryEven"

#include <01Base/Memory/IMemory.h>
#include <03Technical/MemoryManager/MemoryObject.h>

#include <03Technical/MemoryManager/PageList.h>
#include <03Technical/MemoryManager/SlotList.h>

class MemoryEven :public MemoryObject, public IMemory
{
public:
	void* operator new(size_t szThis, const char* sMessage);
	void operator delete(void* pObject);
	void operator delete(void* pObject, const char* sMessage);

private:
	// attributes
	unsigned m_szSlot;
	SlotList* m_pSlotList;
	PageList* m_pPageList;

protected:
	// critical section
	virtual void Lock() = 0;
	virtual void UnLock() = 0;

	void* Malloc(const char* pcName = "");
	void Free(void* pObject);

public:
	// constructors and destructors
	MemoryEven(
		unsigned szSlot,
		PageList* pPageList,
		int nClassId = _MemoryEven_Id,
		const char* pClassName = _MemoryEven_Name);
	virtual ~MemoryEven();;

	virtual void Initialize();
	virtual void Finalize();

	// methods
	void* SafeMalloc(size_t szObject, const char* sMessage = "") override;
	void SafeFree(void* pObject) override;

	// maintenance
	void Show(const char* pTitle) override;
};
