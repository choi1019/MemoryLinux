#pragma once

#include <22TestApplication/typedef.h>
#define _PTS1_Id _GET_TESTLAYER_UID(_ELayer_TestApplication::_ePTS1)
#define _PTS1_Name "PTS1"
#include <22TestApplication/PTS1/Config.h>

#include <21TestPlatform/TestCase/TestSuite.h>
#include <13PTechnical/PMemoryManager/PMemorySystem.h>
#include <13PTechnical/PMemoryManager/PMemoryVariable.h>

#include <22TestApplication/PTS1/PTC11/PTC11.h>
#include <22TestApplication/PTS1/PTC12/PTC12.h>
#include <22TestApplication/PTS1/PTC13/PTC13.h>
#include <22TestApplication/PTS1/PTC14/PTC14.h>

template <int SIZE_SYSTEM_MEMORY, int SIZE_USER_MEMORY, int SIZE_PAGE, int SIZE_SLOT_UNIT>
class PTS1: public TestSuite {
private:
	PMemorySystem* m_pMemorySystem;
	PMemoryVariable* m_pMemoryVariable;

	size_t m_szSystemMemory;
	char* m_pSystemMemeoryAllocated;

	size_t m_szUserMemory;
	char* m_pUserMemeoryAllocated;

public:
	PTS1(
		unsigned classId = _PTS1_Id,
		const char* pClassName = _PTS1_Name)
		: TestSuite(classId, pClassName)
		, m_pMemorySystem(nullptr)
		, m_pMemoryVariable(nullptr)
		, m_pSystemMemeoryAllocated(nullptr)
		, m_pUserMemeoryAllocated(nullptr)
	{
	}
	virtual ~PTS1() {
	}

	void Initialize() {
		try {
			// system memory allocation
			m_szSystemMemory = SIZE_SYSTEM_MEMORY;
			m_pSystemMemeoryAllocated = new char[m_szSystemMemory];
			m_pMemorySystem = new(m_pSystemMemeoryAllocated, m_szSystemMemory) PMemorySystem();
			m_pMemorySystem->Initialize();
			m_pMemorySystem->Show("m_pMemory::Initialize()");

			// aplication memorty allocation
			m_szUserMemory = SIZE_USER_MEMORY;
			m_pUserMemeoryAllocated = new char[m_szUserMemory];
			m_pMemoryVariable = new(m_pUserMemeoryAllocated, m_szUserMemory) PMemoryVariable(SIZE_PAGE, SIZE_SLOT_UNIT);
			m_pMemoryVariable->Initialize();
			m_pMemoryVariable->Show("m_pMemory::Initialize()");

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

	void Finalize() {
		try {
			// delete m_pMemory;
			m_pMemoryVariable->Finalize();
			m_pMemoryVariable->Show("");
			delete m_pMemoryVariable;
			delete[] m_pUserMemeoryAllocated;

			m_pMemorySystem->Finalize();
			m_pMemorySystem->Show("");
			delete m_pMemorySystem;
			delete[] m_pSystemMemeoryAllocated;
		}
		catch (Exception& exception) {
			exception.Println();
			exit(2);
		}
	}
};

