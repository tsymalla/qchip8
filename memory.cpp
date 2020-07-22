#include "memory.h"
#include <assert.h>

namespace Chip8
{
    Memory::Memory()
    {
        resetMemory();
    }

    void Memory::resetMemory()
    {
        _memory = {};
    }

    void Memory::writeByte(size_t offset, std::byte byte)
    {
        assert(offset >= 0 && offset < MEMORY_SIZE);
        _memory[offset] = byte;
    }

    std::byte Memory::readByte(const size_t offset) const
    {
        assert(offset >= 0 && offset < MEMORY_SIZE);
        return _memory[offset];
    }

    std::byte &Memory::readByte(const size_t offset)
    {
        assert(offset >= 0 && offset < MEMORY_SIZE);
        return _memory[offset];
    }

    std::byte Memory::operator[](const size_t offset) const
    {
        return readByte(offset);
    }

    std::byte& Memory::operator[](const size_t offset)
    {
        return readByte(offset);
    }
}
