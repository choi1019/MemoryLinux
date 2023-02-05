#pragma once

#include <01Base/typedef.h>
#define _BaseObject_Id _GET_CLASS_UID(_ELayer_Base::_eBaseObject)
#define _BaseObject_Name "BaseObject"

//#include "../../01Base/Aspect/Log.h"
#include <01Base/Object/RootObject.h>
#include <01Base/Memory/IMemory.h>

class BaseObject : public RootObject {
public:
	static IMemory* s_pMemory;

	void* operator new (size_t szThis, const char* sMessage);
	void* operator new[] (size_t szThis, const char* sMessage);
	void operator delete(void* pObject);
	void operator delete[](void* pObject);

	// dummy
	void operator delete(void* pObject, const char* sMessage);
	void operator delete[](void* pObject, const char* sMessage);

public:
	// constructors & destructors
	BaseObject(
		int nClassId = _BaseObject_Id,
		const char* pcClassName = _BaseObject_Name)
		: RootObject(nClassId, pcClassName) {
	}
	virtual ~BaseObject() {
	}
	
	virtual void Initialize() {
		RootObject::Initialize();
	}
	
	virtual void Finalize() {
		RootObject::Finalize();
	}

	// cloneable
	virtual BaseObject* Clone() {
		return nullptr;
	}

	// serializable
	virtual char* Serialize() {
		return nullptr;
	}
	virtual void DeSerialize(char* pBuffer) {
	}
};
