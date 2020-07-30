#ifndef CPU_H
#define CPU_H

#include <QObject>
#include "datatypes.h"
#include "memory.h"
#include "opcodeinterpreter.h"
#include <QMutex>

namespace Chip8
{
    class CPU: public QObject
    {
        Q_OBJECT

        constexpr static size_t REGISTER_COUNT = 16;
        constexpr static size_t STACK_SIZE = 16;
        constexpr static size_t KEY_COUNT = 16;

    public:
        CPU(QString filename, QObject* parent = nullptr);
        void setROM(QString filename);
        void loadROM();

        void reset();
        void run();
        void stop();
        bool isRunning() const;
        void _stepProgramCounterByte();
        void _stepProgramCounterWord();

    signals:
        void refreshScreen(FrameBuffer framebuffer);

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
        StaticWordArray<STACK_SIZE> _stack;
        Word _stackPointer;

        FrameBuffer _framebuffer;
        StaticByteArray<KEY_COUNT> _keyStatus;

        OpcodeInterpreter _opcodeInterpreter;

        QMutex _mutex;

        void _cycle();
    };
}

#endif // CPU_H
