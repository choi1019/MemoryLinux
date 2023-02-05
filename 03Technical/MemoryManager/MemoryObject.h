#pragma once

#include <03Technical/typedef.h>
#define _MemoryObject_Id _GET_CLASS_UID(_ELayer_Technical::_eMemoryObject)
#define _MemoryObject_Name "MemoryObject"

#include <01Base/Object/RootObject.h>

class MemoryObject : public RootObject {
public:
	// system memory allocated
	static IMemory* s_pMemory;

	void* operator new (size_t szThis, const char* sMessage);
	void* operator new[] (size_t szThis, const char* sMessage);
	void operator delete(void* pObject);
	void operator delete[](void* pObject);

	// dummy
	void operator delete(void* pObject, const char* sMessage);
	void operator delete[](void* pObject, const char* sMessage);
	
	MemoryObject(
		int nClassId = _MemoryObject_Id,
		const char* pClassName = _MemoryObject_Name)
		: RootObject(nClassId, pClassName)
	{
	}
	virtual ~MemoryObject() {
	}
	virtual void Initialize() {
	}
	virtual void Finalize() {
	}
};