#include <22TestApplication/PTS1/PTS1.h>

	PTS1::PTS1(
		unsigned classId = _PTS1_Id,
		const char* pClassName = _PTS1_Name)
		: TestSuite(classId, pClassName)
		, m_pMemory(nullptr)
		, m_pSystemMemeoryAllocated(nullptr)
		, m_pUserMemeoryAllocated(nullptr)
	{
	}
	PTS1::~PTS1() {
	}

	void PTS1::Initialize() {
		try {
			// system memory allocation
			m_szSystemMemory = SIZE_SYSTEM_MEMORY;
			m_pSystemMemeoryAllocated = new char[m_szSystemMemory];

			// aplication memorty allocation
			m_szUserMemory = SIZE_USER_MEMORY;
			m_pUserMemeoryAllocated = new char[m_szUserMemory];

			m_pMemory = new(m_pSystemMemeoryAllocated, m_szSystemMemory) PMemory(m_pUserMemeoryAllocated, m_szUserMemory, SIZE_PAGE, SIZE_SLOT_UNIT);

			m_pMemory->Initialize();
			m_pMemory->Show("m_pMemory::Initialize()");

			this->add(new("PTC11") PTC11());
			this->add(new("PTC12") PTC12());
			this->add(new("PTC13") PTC13());
			this->add(new("PTC14") PTC14());
			this->add(new("PTC11") PTC11());
			this->add(new("PTC12") PTC12());
		}
		catch (Exception& exception) {
			exception.Println();
			exit(1);
		}
	}

	void PTS1::Finalize() {
		try {
			m_pMemory->Finalize();
			m_pMemory->GetPPageList()->Show("");
			m_pMemory->Show("");

			// delete m_pMemory;
			delete[] m_pUserMemeoryAllocated;
			delete[] m_pSystemMemeoryAllocated;
		}
		catch (Exception& exception) {
			exception.Println();
			exit(2);
		}
	}