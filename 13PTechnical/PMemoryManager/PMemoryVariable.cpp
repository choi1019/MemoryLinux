#include <13PTechnical/PMemoryManager/PMemoryVariable.h>

	void PMemoryVariable::Lock() {
//		EnterCriticalSection(&CriticalSection);
        pthread_mutex_lock(&m_mutex);
	}
	void PMemoryVariable::UnLock() {
//		LeaveCriticalSection(&CriticalSection);
        pthread_mutex_unlock(&m_mutex);
	}

	PMemoryVariable::PMemoryVariable(
		size_t szPage,
		size_t szSlotUnit,
		unsigned nClassId,
		const char* pcClassName)
		: MemoryVariable(szPage, szSlotUnit, nClassId, pcClassName)
	{
//		InitializeCriticalSection(&CriticalSection);
	}
	PMemoryVariable::~PMemoryVariable() {
//		DeleteCriticalSection(&CriticalSection);
	}

	void PMemoryVariable::Initialize() {
        pthread_mutex_init(&m_mutex, nullptr);
		MemoryVariable::Initialize();
	}
	void PMemoryVariable::Finalize() {
		MemoryVariable::Finalize();
        pthread_mutex_destroy(&m_mutex);
	}