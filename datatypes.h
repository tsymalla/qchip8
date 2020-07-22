#ifndef DATATYPES_H
#define DATATYPES_H

#include <array>

namespace Chip8
{
    template<typename T, size_t Size>
    using StaticArray = std::array<T, Size>;

    using Word = uint16_t;
    using Byte = uint8_t;

    template<size_t Size>
    using StaticByteArray = StaticArray<Byte, Size>;
}

#endif // DATATYPES_H
