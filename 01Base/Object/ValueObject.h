#pragma once

#include <01Base/typedef.h>
#define _ValueObject_Id _GET_CLASS_UID(_ELayer_Base::_eValueObject)
#define _ValueObject_Name "ValueObject"

//#include "../../01Base/Aspect/Log.h"
#include <01Base/Object/RootObject.h>
#include <01Base/Memory/IMemory.h>

class ValueObject : public RootObject {
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
	ValueObject(
		int nClassId = _ValueObject_Id,
		const char* pcClassName = _ValueObject_Name)
		: RootObject(nClassId, pcClassName) {
	}
	virtual ~ValueObject() {
	}
	
	virtual void Initialize() {
		RootObject::Initialize();
	}
	
	virtual void Finalize() {
		RootObject::Finalize();
	}

	// cloneable
	virtual ValueObject* Clone() {
		return nullptr;
	}

	// serializable
	virtual char* Serialize() {
		return nullptr;
	}
	virtual void DeSerialize(char* pBuffer) {
	}
};
