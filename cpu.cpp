#include "cpu.h"
#include <utility>

namespace Chip8
{
    CPU::CPU(QString filename): _filename(std::move(filename))
    {
    }

    void CPU::loadROM()
    {
        _memory.resetMemory();
    }
}
