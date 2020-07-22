#ifndef CPU_H
#define CPU_H

#include <QObject>
#include "memory.h"

namespace Chip8
{
    class CPU: public QObject
    {
        Q_OBJECT

    public:
        CPU(QString filename);
        void loadROM();

    private:
        QString _filename;
        Memory _memory;
    };
}

#endif // CPU_H
