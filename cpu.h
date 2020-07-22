#ifndef CPU_H
#define CPU_H

#include <QObject>
#include "datatypes.h"
#include "memory.h"
#include <stack>

namespace Chip8
{
    class CPU: public QObject
    {
        Q_OBJECT

        constexpr static size_t REGISTER_COUNT = 16;
        constexpr static size_t STACK_SIZE = 16;
        constexpr static size_t DISPLAY_WIDTH = 64;
        constexpr static size_t DISPLAY_HEIGHT = 32;
        constexpr static size_t DISPLAY_SIZE = DISPLAY_WIDTH * DISPLAY_HEIGHT;
        constexpr static size_t KEY_COUNT = 16;

    public:
        CPU(QString filename);
        void loadROM();

        void reset();
        void run();
        bool isRunning() const;

    signals:
        void romLoadingFailed();
        void romLoaded();
        void refreshScreen(StaticByteArray<DISPLAY_SIZE>& framebuffer);

    private:
        QString _filename;
        bool _isRunning;
        bool _canRefreshScreen;

        Memory _memory;
        StaticByteArray<REGISTER_COUNT> _baseRegisters;
        Word _addressRegister;
        Word _programCounter;
        Byte _delayTimer;
        Byte _soundTimer;
        Word _opcode;
        std::stack<Word> _stack;
        Word _stackPointer;

        StaticByteArray<DISPLAY_SIZE> _framebuffer;
        StaticByteArray<KEY_COUNT> _keyStatus;

        void _cycle();
        void _clearStack();
    };
}

#endif // CPU_H
