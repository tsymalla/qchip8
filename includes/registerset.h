#ifndef REGISTERSET_H
#define REGISTERSET_H

#include "datatypes.h"

namespace Chip8
{
    class RegisterSet
    {
        StaticByteArray<REGISTER_COUNT> _baseRegisters;
        Word _addressRegister;
        StaticWordArray<STACK_SIZE> _stack;
        Word _stackPointer;
        Byte _delayTimer;
        Byte _soundTimer;

    public:
        void reset();

        void pushStack(Word value);
        Word popStack();

        void setRegisterValue(size_t index, Byte value);
        void addRegisterValue(size_t index, Byte value);
        void subRegisterValue(size_t index, Byte value);
        void shrRegisterValue(size_t index, size_t count);
        void shlRegisterValue(size_t index, size_t count);
        Byte getRegisterValue(size_t index) const;

        void setAddressRegister(Word value);
        void incAddressRegister(Word value);
        Word getAddressRegister() const;

        void setDelayTimer(Byte value);
        void decDelayTimer();
        Byte getDelayTimer() const;

        void setSoundTimer(Byte value);
        void decSoundTimer();
        Byte getSoundTimer() const;
    };
}

#endif // REGISTERSET_H
