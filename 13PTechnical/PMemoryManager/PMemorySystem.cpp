#include <13PTechnical/PMemoryManager/PMemorySystem.h>

	void PMemorySystem::Lock() {
//		EnterCriticalSection(&CriticalSection);
        pthread_mutex_lock(&m_mutex);
	}
	void PMemorySystem::UnLock() {
//		LeaveCriticalSection(&CriticalSection);
        pthread_mutex_unlock(&m_mutex);
	}

	PMemorySystem::PMemorySystem(
		unsigned nClassId,
		const char* pcClassName)
		: MemorySystem(nClassId, pcClassName)
	{
	}
	PMemorySystem::~PMemorySystem() {
//		DeleteCriticalSection(&CriticalSection);
	}

	void PMemorySystem::Initialize() {
        pthread_mutex_init(&m_mutex, nullptr);
		MemorySystem::Initialize();
	}
	void PMemorySystem::Finalize() {
		MemorySystem::Finalize();
        pthread_mutex_destroy(&m_mutex);
	}