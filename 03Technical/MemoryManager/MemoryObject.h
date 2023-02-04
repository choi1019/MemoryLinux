#pragma once

#include <03Technical/typedef.h>
#define _MemoryObject_Id _GET_CLASS_UID(_ELayer_Technical::_eMemoryObject)
#define _MemoryObject_Name "MemoryObject"

#include <01Base/Object/RootObject.h>

class MemoryObject : public RootObject {
public:
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