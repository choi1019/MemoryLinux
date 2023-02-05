#pragma once

#include <13PTechnical/typedef.h>
#define _PMemoryStatic_Id _GET_CLASS_UID(_ELayer_PTechnical::_ePMemoryStatic)
#define _PMemoryStatic_Name "PMemoryStatic"

#include <03Technical/MemoryManager/MemoryStatic.h>
#include <pthread.h>

class PMemoryStatic : public MemoryStatic {
private:
//	CRITICAL_SECTION CriticalSection;
	pthread_mutex_t m_mutex;

protected:
	void Lock() override;
	void UnLock() override;

public:
	PMemoryStatic(
		unsigned nClassId = _PMemoryStatic_Id,
		const char* pcClassName = _PMemoryStatic_Name);
	~PMemoryStatic() override;

	virtual void Initialize() override;
	virtual void Finalize() override;
};