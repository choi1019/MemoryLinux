#pragma once

#include <01Base/Object/ValueObject.h>
#include <01Base/Aspect/Log.h>
#include <01Base/StdLib/Vector.h>

class DomainObject11 : public ValueObject {
private:
	Vector<int>* m_pVector;  // 248 Byte

public:
	DomainObject11() 
		: ValueObject()
		, m_pVector(0)
	{
		m_pVector = new("m_pVector") Vector<int>();
		ValueObject::s_pMemory->Show("DomainObject11");
	}
	~DomainObject11() {
		delete this->m_pVector;
	}

	void Run() {
		for (int i = 0; i < this->m_pVector->Max_size(); i++) {
			this->m_pVector->Add(i);
		}
	}
};
