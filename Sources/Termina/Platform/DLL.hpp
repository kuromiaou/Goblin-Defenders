#pragma once

namespace Termina {
    class DLL {
    public:
        DLL() = default;
        DLL(const char* path);
        ~DLL();

        void Load(const char* path);
        void* GetSymbol(const char* name);

    private:
        void* m_Handle = nullptr;
    };
}
