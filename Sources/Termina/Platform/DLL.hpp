#pragma once

namespace Termina {
    /// Represents a dynamically loaded library (DLL or shared object).
    class DLL
    {
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
