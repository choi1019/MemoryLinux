#include <13PTechnical/PMemoryManager/PMemory.h>

	void PMemory::Lock() {
//		EnterCriticalSection(&CriticalSection);
        pthread_mutex_lock(&m_mutex);
	}
	void PMemory::UnLock() {
//		LeaveCriticalSection(&CriticalSection);
        pthread_mutex_unlock(&m_mutex);
	}

	PMemory::PMemory(
		void* pMemeoryAllocated,
		size_t szMemoryAllocated,
		size_t szPage,
		size_t szSlotUnit,

		unsigned nClassId,
		const char* pcClassName)
		: Memory(pMemeoryAllocated, szMemoryAllocated, szPage, szSlotUnit, nClassId, pcClassName)
	{
//		InitializeCriticalSection(&CriticalSection);
	}
	PMemory::~PMemory() {
//		DeleteCriticalSection(&CriticalSection);
	}

	void PMemory::Initialize() {
        pthread_mutex_init(&m_mutex, nullptr);
		Memory::Initialize();
	}
	void PMemory::Finalize() {
		Memory::Finalize();
        pthread_mutex_destroy(&m_mutex);
	}