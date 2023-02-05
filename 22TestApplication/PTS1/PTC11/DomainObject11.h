#pragma once

#include <01Base/Object/BaseObject.h>
#include <01Base/Aspect/Log.h>
#include <01Base/StdLib/Vector.h>

class DomainObject11 : public BaseObject {
private:
	Vector<int>* m_pVector;  // 248 Byte

public:
	DomainObject11() 
		: BaseObject()
		, m_pVector(0)
	{
		m_pVector = new("m_pVector") Vector<int>();
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
