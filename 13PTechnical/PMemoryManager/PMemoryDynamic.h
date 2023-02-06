#pragma once

#include <13PTechnical/typedef.h>
#define _PMemoryDynamic_Id _GET_CLASS_UID(_ELayer_PTechnical::_ePMemoryDynamic)
#define _PMemoryDynamic_Name "PMemoryDynamic"

#include <03Technical/MemoryManager/MemoryDynamic.h>
#include <pthread.h>

class PMemoryDynamic : public MemoryDynamic {
private:
//	CRITICAL_SECTION CriticalSection;
	pthread_mutex_t m_mutex;

protected:
	void Lock() override;
	void UnLock() override;

public:
	PMemoryDynamic(
		size_t szPage,
		size_t szSlotUnit,
		unsigned nClassId = _PMemoryDynamic_Id,
		const char* pcClassName = _PMemoryDynamic_Name);
	~PMemoryDynamic() override;

	virtual void Initialize() override;
	virtual void Finalize() override;
};