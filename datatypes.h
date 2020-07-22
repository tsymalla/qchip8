#ifndef DATATYPES_H
#define DATATYPES_H

#include <QMetaType>
#include <array>

namespace Chip8
{
    template<typename T, size_t Size>
    using StaticArray = std::array<T, Size>;

    using Word = uint16_t;
    using Byte = uint8_t;

    template<size_t Size>
    using StaticByteArray = StaticArray<Byte, Size>;

    template<size_t Size>
    using StaticWordArray = StaticArray<Word, Size>;

    constexpr static size_t DISPLAY_WIDTH = 64;
    constexpr static size_t DISPLAY_HEIGHT = 32;
    constexpr static size_t DISPLAY_SIZE = DISPLAY_WIDTH * DISPLAY_HEIGHT;

    using FrameBuffer = StaticByteArray<DISPLAY_SIZE>;
}

Q_DECLARE_METATYPE(Chip8::FrameBuffer);

#endif // DATATYPES_H
