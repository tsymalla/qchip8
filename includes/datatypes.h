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

    constexpr static size_t SPRITE_WIDTH = 8;

    constexpr static size_t REGISTER_COUNT = 16;
    constexpr static size_t STACK_SIZE = 16;
    constexpr static size_t KEY_COUNT = 16;

    using FrameBuffer = StaticByteArray<DISPLAY_SIZE>;
    using KeyBuffer = StaticArray<bool, KEY_COUNT>;
}

Q_DECLARE_METATYPE(Chip8::FrameBuffer);

#endif // DATATYPES_H
