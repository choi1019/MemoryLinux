#pragma once

#include <13PTechnical/typedef.h>
#define _PMemory_Id _GET_CLASS_UID(_ELayer_PTechnical::_ePMemory)
#define _PMemory_Name "PMemory"

#include <03Technical/MemoryManager/MemoryVariable.h>
#include <pthread.h>

class PMemoryVariable : public MemoryVariable {
private:
//	CRITICAL_SECTION CriticalSection;
	pthread_mutex_t m_mutex;

protected:
	void Lock() override;
	void UnLock() override;

public:
	PMemoryVariable(
		void* pMemeoryAllocated,
		size_t szMemoryAllocated,
		size_t szPage,
		size_t szSlotUnit,
		unsigned nClassId = _PMemory_Id,
		const char* pcClassName = _PMemory_Name);
	~PMemoryVariable() override;

	virtual void Initialize() override;
	virtual void Finalize() override;
};