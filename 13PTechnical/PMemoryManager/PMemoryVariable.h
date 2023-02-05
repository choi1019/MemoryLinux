#pragma once

#include <13PTechnical/typedef.h>
#define _PMemoryVariable_Id _GET_CLASS_UID(_ELayer_PTechnical::_ePMemoryVariable)
#define _PMemoryVariable_Name "PMemoryVariable"

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
		size_t szPage,
		size_t szSlotUnit,
		unsigned nClassId = _PMemoryVariable_Id,
		const char* pcClassName = _PMemoryVariable_Name);
	~PMemoryVariable() override;

	virtual void Initialize() override;
	virtual void Finalize() override;
};