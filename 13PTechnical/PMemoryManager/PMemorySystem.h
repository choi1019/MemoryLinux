#pragma once

#include <13PTechnical/typedef.h>
#define _PMemory_Id _GET_CLASS_UID(_ELayer_PTechnical::_ePMemory)
#define _PMemory_Name "PMemory"

#include <03Technical/MemoryManager/MemorySystem.h>
#include <pthread.h>

class PMemorySystem : public MemorySystem {
private:
//	CRITICAL_SECTION CriticalSection;
	pthread_mutex_t m_mutex;

protected:
	void Lock() override;
	void UnLock() override;

public:
	PMemorySystem(
		unsigned nClassId = _PMemory_Id,
		const char* pcClassName = _PMemory_Name);
	~PMemorySystem() override;

	virtual void Initialize() override;
	virtual void Finalize() override;
};