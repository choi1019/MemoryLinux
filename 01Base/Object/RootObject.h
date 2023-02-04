#pragma once

#include <01Base/typedef.h>

#define _RootObject_Id _GET_CLASS_UID(_ELayer_Base::_eRootObject)
#define _RootObject_Name "RootObject"

#include <01Base/Memory/IMemory.h>

class RootObject {
public:
		enum class EState {
		eCreated,
		eInitialized,
		eStarted,
		eStopped,
		eFinalized,
		eDeleted,
		eEnd
	};
	static unsigned s_uObjectCount;

	// system memory allocated
	static IMemory* s_pMemory;

	void* operator new (size_t szThis, const char* sMessage);
	void* operator new[] (size_t szThis, const char* sMessage);
	void operator delete(void* pObject);
	void operator delete[](void* pObject);

	// dummy
	void operator delete(void* pObject, const char* sMessage);
	void operator delete[](void* pObject, const char* sMessage);
	
private:
	// attributes
	unsigned 	m_uObjectId;
	unsigned 	m_nClassId;
	const char* m_pcClassName;
	size_t 		m_szThis;
	EState 		m_eState;

public:
	// getters and setters
	inline int GetObjectId() { return this->m_uObjectId; }
	inline int GetClassId() { return this->m_nClassId; }
	inline const char* GetClassName() { return this->m_pcClassName; }

	inline size_t GetSzThis() { return this->m_szThis; }
	inline void SetSzThis(size_t szThis) { this->m_szThis = szThis; }
	inline EState GetEState() { return this->m_eState; }
	inline void SetEState(EState eState) { this->m_eState = eState; }

	// constructors and destructors
	RootObject(unsigned nClassId = _RootObject_Id, const char* pcClassName = _RootObject_Name)
	: m_nClassId(nClassId)
	, m_pcClassName(pcClassName)
	, m_szThis(0)
	{
		this->m_eState = EState::eCreated;
		this->m_uObjectId = RootObject::s_uObjectCount++;
	}
	virtual ~RootObject() 
	{
	    this->m_eState = EState::eDeleted;
	}
	virtual void Initialize()
	{
		this->m_eState = EState::eInitialized;
	}
	virtual void Finalize()
	{
		this->m_eState = EState::eFinalized;
	}
};

