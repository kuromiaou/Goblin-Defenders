#pragma once

#include <vector>

#include "Common.hpp"

namespace Termina {
    /// A simple free list implementation for managing a fixed number of slots.
    class FreeList
    {
    public:
        static const int INVALID = -1;

        FreeList(uint64 maxSlots);
        ~FreeList();

        /// Allocates a slot and returns its index. Returns `INVALID` if no slots are available.
        int Allocate();

        /// Frees a slot by its index.
        void Free(int index);
    private:
        void SetBit(int index);
        void ClearBit(int index);
        bool IsBitSet(int index) const;

        uint64 m_MaxSlots;
        uint64 m_BitmapSize;
        std::vector<uint64> m_Bitmap;
        std::vector<uint32> m_FreeList;
    };
}
