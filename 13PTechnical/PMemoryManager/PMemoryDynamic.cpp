#include <13PTechnical/PMemoryManager/PMemoryDynamic.h>

	void PMemoryDynamic::Lock() {
//		EnterCriticalSection(&CriticalSection);
        pthread_mutex_lock(&m_mutex);
	}
	void PMemoryDynamic::UnLock() {
//		LeaveCriticalSection(&CriticalSection);
        pthread_mutex_unlock(&m_mutex);
	}

	PMemoryDynamic::PMemoryDynamic(
		size_t szPage,
		size_t szSlotUnit,
		unsigned nClassId,
		const char* pcClassName)
		: MemoryDynamic(szPage, szSlotUnit, nClassId, pcClassName)
	{
//		InitializeCriticalSection(&CriticalSection);
	}
	PMemoryDynamic::~PMemoryDynamic() {
//		DeleteCriticalSection(&CriticalSection);
	}

	void PMemoryDynamic::Initialize() {
        pthread_mutex_init(&m_mutex, nullptr);
		MemoryDynamic::Initialize();
	}
	void PMemoryDynamic::Finalize() {
		MemoryDynamic::Finalize();
        pthread_mutex_destroy(&m_mutex);
	}