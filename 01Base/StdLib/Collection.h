#pragma once

#include <01Base/typedef.h>
#define _Collection_Id _GET_CLASS_UID(_ELayer_Base::_eCollection)
#define _Collection_Name "Collection"

#include <01Base/Object/ValueObject.h>

class Collection : public ValueObject {
public:
	enum class EError
	{
		_eBegin = _Collection_Id,
		_eIndexOverflow,
		_eEnd
	};

public:
	Collection(int nClassId = _Collection_Id, const char* cClassName = _Collection_Name)
		: ValueObject(nClassId, cClassName)
	{
	}
	virtual ~Collection() {
	}
	virtual void Initialize() {
		ValueObject::Initialize();
	}
	virtual void Finalize() {
		ValueObject::Finalize();
	}
};