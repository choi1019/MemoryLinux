#pragma once

#include <03Technical/typedef.h>
#define _MemorySystem_Id _GET_CLASS_UID(_ELayer_Technical::_eMemorySystem)
#define _MemorySystem_Name "MemorySystem"

#include <01Base/Object/RootObject.h>
#include <01Base/Memory/IMemory.h>

class MemorySystem : public RootObject, public IMemory {
public:
	// system memory allocated
	static void* s_pMemoryAllocated;
	static size_t s_szMemoryAllocated;
	static void* s_pMemoryCurrent;
	static size_t s_szMemoryCurrent;

	void* operator new(size_t szThis, void* pMemoryAllocated, size_t szMemoryllocated);
	void operator delete(void* pObject);
	void operator delete(void* pObject, void* pMemoryAllocated, size_t szMemoryllocated);

protected:
	virtual void Lock() = 0;
	virtual void UnLock() = 0;

	void* Malloc(size_t szObject, const char* pcName = "");
	void Free(void* pObject);

public:
	MemorySystem(
		int nClassId = _MemorySystem_Id,
		const char* pClassName = _MemorySystem_Name);
	virtual ~MemorySystem();
	virtual void Initialize();
	virtual void Finalize();

		// methods
	void* SafeMalloc(size_t szAllocate, const char* pcName = "");
	void SafeFree(void* pObject);

	// maintenance
	virtual void Show(const char* pTitle);
};