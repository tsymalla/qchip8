#ifndef CPU_H
#define CPU_H

#include <QObject>
#include <QAtomicInteger>
#include <map>
#include "datatypes.h"
#include "memory.h"
#include "registerset.h"
#include "is.h"

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

        void keyDown(int key);
        void keyUp(int key);

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
        IS *_is;

        FrameBuffer _framebuffer;
        KeyBuffer _keyStatus;

        void _decode();
        void _execute();
        void _cycle();

        inline const static std::map<int, int> KEY_MAP = {
            {
                Qt::Key_1, 0x1
            },
            {
                Qt::Key_2, 0x2
            },
            {
                Qt::Key_3, 0x3
            },
            {
                Qt::Key_4, 0xC
            },
            {
                Qt::Key_Q, 0x4
            },
            {
                Qt::Key_W, 0x5
            },
            {
                Qt::Key_E, 0x6
            },
            {
                Qt::Key_R, 0xD
            },
            {
                Qt::Key_A, 0x7
            },
            {
                Qt::Key_S, 0x8
            },
            {
                Qt::Key_D, 0x9
            },
            {
                Qt::Key_F, 0xE
            },
            {
                Qt::Key_Z, 0xA
            },
            {
                Qt::Key_X, 0x0
            },
            {
                Qt::Key_C, 0xB
            },
            {
                Qt::Key_V, 0xF
            }
        };
    };
}

#endif // CPU_H
