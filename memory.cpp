#include "memory.h"
#include <assert.h>

namespace Chip8
{
    Memory::Memory()
    {
        resetMemory();
    }

    void Memory::_loadFontMap()
    {
        constexpr static size_t FONTMAP_SIZE = 80;
        constexpr static StaticByteArray<FONTMAP_SIZE> fontMap = {
            0xF0, 0x90, 0x90, 0x90, 0xF0,   // 0
            0x20, 0x60, 0x20, 0x20, 0x70,   // 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0,   // 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0,   // 3
            0x90, 0x90, 0xF0, 0x10, 0x10,   // 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0,   // 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0,   // 6
            0xF0, 0x10, 0x20, 0x40, 0x40,   // 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0,   // 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0,   // 9
            0xF0, 0x90, 0xF0, 0x90, 0x90,   // A
            0xE0, 0x90, 0xE0, 0x90, 0xE0,   // B
            0xF0, 0x80, 0x80, 0x80, 0xF0,   // C
            0xE0, 0x90, 0x90, 0x90, 0xE0,   // D
            0xF0, 0x80, 0xF0, 0x80, 0xF0,   // E
            0xF0, 0x80, 0xF0, 0x80, 0x80    // F
        };

        for (size_t i = 0; i < FONTMAP_SIZE; ++i)
        {
            _memory[i] = fontMap[i];
        }
    }

    void Memory::resetMemory()
    {
        _memory.fill(0x00);
        _loadFontMap();
    }

    void Memory::writeByte(size_t offset, Byte byte)
    {
        assert(offset >= 0 && offset < MEMORY_SIZE);
        _memory[offset] = byte;
    }

    Byte Memory::readByte(const size_t offset) const
    {
        assert(offset >= 0 && offset < MEMORY_SIZE);
        return _memory[offset];
    }

    Byte& Memory::readByte(const size_t offset)
    {
        assert(offset >= 0 && offset < MEMORY_SIZE);
        return _memory[offset];
    }

    Byte Memory::operator[](const size_t offset) const
    {
        return readByte(offset);
    }

    Byte& Memory::operator[](const size_t offset)
    {
        return readByte(offset);
    }

    Memory& Memory::operator=(const QByteArray &data)
    {
        for (size_t i = ROM_START; i < static_cast<size_t>(data.size()); ++i)
        {
            _memory[i] = data.at(static_cast<int>(i - ROM_START));
        }

        return *this;
    }
}
