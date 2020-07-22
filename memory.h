#ifndef MEMORY_H
#define MEMORY_H

#include <array>

namespace Chip8
{
    class Memory
    {
    private:
        constexpr static unsigned int MEMORY_SIZE = 4096;

        std::array<std::byte, MEMORY_SIZE> _memory;
    public:
        Memory();
        void resetMemory();

        void writeByte(size_t offset, std::byte byte);
        std::byte readByte(const size_t offset) const;
        std::byte& readByte(const size_t offset);

        std::byte operator[](const size_t offset) const;
        std::byte& operator[](const size_t offset);
    };
}

#endif // MEMORY_H
