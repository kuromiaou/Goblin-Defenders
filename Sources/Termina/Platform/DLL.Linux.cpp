#include "DLL.hpp"
#include "Core/Logger.hpp"

#if defined(TRMN_LINUX)

#include <dlfcn.h>

namespace Termina {
    DLL::DLL(const char* path)
    {
        Load(path);
    }
    
    DLL::~DLL()
    {
        if (m_Handle) {
            dlclose(m_Handle);
        }
    }
    
    void DLL::Load(const char* path)
    {
        if (m_Handle) {
            dlclose(m_Handle);
        }
        m_Handle = dlopen(path, RTLD_LAZY);
        if (!m_Handle) {
            TN_FATAL("Failed to load DLL %s: %s", path, dlerror());
        }
    }

    void* DLL::GetSymbol(const char* name)
    {
        if (m_Handle) {
            return dlsym(m_Handle, name);
        }
        return nullptr;
    }
}

#endif
