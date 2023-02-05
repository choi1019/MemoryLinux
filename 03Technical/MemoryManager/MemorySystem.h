#pragma once

#include <03Technical/typedef.h>
#define _MemorySystem_Id _GET_CLASS_UID(_ELayer_Technical::_eMemorySystem)
#define _MemorySystem_Name "MemorySystem"

#include <01Base/Memory/IMemory.h>
#include <03Technical/MemoryManager/MemoryObject.h>

class MemorySystem : public MemoryObject, public IMemory {
public:
	// memory for Memory Manager
	static void* s_pAllocated;
	static size_t s_szAllocated;
	static void* s_pCurrent;
	static size_t s_szCurrent;

	void* operator new(size_t szThis, void* pMemoryAllocated, size_t szMemoryllocated);
	void operator delete(void* pObject);
	void operator delete(void* pObject, void* pMemoryAllocated, size_t szMemoryllocated);

protected:
	virtual void Lock() = 0;
	virtual void UnLock() = 0;

	void* Malloc(size_t szObject, const char* sMessage = "");
	void Free(void* pObject);

public:
	MemorySystem(
		int nClassId = _MemorySystem_Id,
		const char* pClassName = _MemorySystem_Name);
	virtual ~MemorySystem();
	virtual void Initialize();
	virtual void Finalize();

		// methods
	void* SafeMalloc(size_t szAllocate, const char* sMessage = "") override;
	void SafeFree(void* pObject) override;

	// maintenance
	void Show(const char* sMessage) override;
};