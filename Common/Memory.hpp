#pragma once

#include <LESDK/Headers.hpp>
#include "Common/Base.hpp"


namespace Common
{

    /**
     * @brief
     * A mix-in ensuring that derived types, when using new / delete operators,
     * are (de)allocated through the game's global allocator (GMalloc).
     */
    class AllocateThroughEngine
    {
    public:

        void* operator new(size_t const Size)
        {
            assert(GMalloc != nullptr);
            return (*GMalloc)->Malloc(static_cast<DWORD>(Size), 16);
        }

        void* operator new[](size_t const Size)
        {
            assert(GMalloc != nullptr);
            return (*GMalloc)->Malloc(static_cast<DWORD>(Size), 16);
        }

        void operator delete(void* const Pointer)
        {
            assert(GMalloc != nullptr);
            (*GMalloc)->Free(Pointer);
        }

        void operator delete[](void* const Pointer)
        {
            assert(GMalloc != nullptr);
            (*GMalloc)->Free(Pointer);
        }
    };

} // namespace Common