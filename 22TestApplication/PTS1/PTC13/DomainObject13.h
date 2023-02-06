#pragma once

#include <01Base/Object/ValueObject.h>
#include <01Base/Aspect/Log.h>
#include <01Base/StdLib/Vector.h>

class DomainObject13 : public ValueObject {
private:
	Vector<int, 100>* m_pVector;  // 248 Byte

public:
	DomainObject13() : ValueObject() {
		this->m_pVector = new("DomainObject13::m_pVector") Vector<int, 100>();
	}
	virtual ~DomainObject13() {
		delete this->m_pVector;
	}

	void Run() {
		for (int i = 0; i < m_pVector->Max_size(); i++) {
			this->m_pVector->Add(i);
		}
	}
};
