#ifndef CPU_H
#define CPU_H

#include <QObject>
#include <QAtomicInteger>
#include "datatypes.h"
#include "memory.h"
#include "registerset.h"
#include "opcodeinterpreter.h"

namespace Chip8
{
    class CPU: public QObject
    {
        Q_OBJECT

    public:
        CPU(QObject* parent = nullptr);
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
        QAtomicInteger<bool> _isRunning;
        bool _canRefreshScreen;

        Memory _memory;
        Word _programCounter;
        Word _opcode;
        RegisterSet _registerSet;

        FrameBuffer _framebuffer;
        StaticByteArray<KEY_COUNT> _keyStatus;

        OpcodeInterpreter _opcodeInterpreter;

        void _cycle();
    };
}

#endif // CPU_H
