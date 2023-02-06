#include "Directory.h"
#include <01Base/Aspect/Log.h>

Directory Directory::s_dirObjects(Directory::EType::eObject);
Directory Directory::s_dirComponents(Directory::EType::eComponent);
Directory Directory::s_dirEvents(Directory::EType::eEvent);
Directory Directory::s_dirExceptions(Directory::EType::eException);

void Directory::ClearDirectories() {
	Directory::s_dirObjects.Clear();
	Directory::s_dirComponents.Clear();
	Directory::s_dirEvents.Clear();
	Directory::s_dirExceptions.Clear();
};

void Directory::Show(const char* pcTitle) {
	LOG_HEADER("Directory", (int)m_eType);
	for (auto itr : *this) {
		LOG_NEWLINE(itr.first, itr.second);
	}
	LOG_FOOTER("Directory", (int)m_eType);
}