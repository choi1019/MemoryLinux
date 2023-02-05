#include <13PTechnical/PMemoryManager/PMemoryStatic.h>

	void PMemoryStatic::Lock() {
//		EnterCriticalSection(&CriticalSection);
        pthread_mutex_lock(&m_mutex);
	}
	void PMemoryStatic::UnLock() {
//		LeaveCriticalSection(&CriticalSection);
        pthread_mutex_unlock(&m_mutex);
	}

	PMemoryStatic::PMemoryStatic(
		unsigned nClassId,
		const char* pcClassName)
		: MemoryStatic(nClassId, pcClassName)
	{
	}
	PMemoryStatic::~PMemoryStatic() {
//		DeleteCriticalSection(&CriticalSection);
	}

	void PMemoryStatic::Initialize() {
        pthread_mutex_init(&m_mutex, nullptr);
		MemoryStatic::Initialize();
	}
	void PMemoryStatic::Finalize() {
		MemoryStatic::Finalize();
        pthread_mutex_destroy(&m_mutex);
	}