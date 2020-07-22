#ifndef EMULATORWORKER_H
#define EMULATORWORKER_H

#include <QObject>
#include <QMutex>
#include "cpu.h"

class EmulatorWorker: public QObject
{
    Q_OBJECT
public:
    explicit EmulatorWorker(QString filename, QObject* parent = nullptr);
    void setROM(QString filename);
    void stopEmulation();

public slots:
    void onRunEmulation();
    void onRefreshScreen(Chip8::FrameBuffer framebuffer);

signals:
    void refreshScreen(Chip8::FrameBuffer framebuffer);
    void finishedEmulation();

private:
    Chip8::CPU* _emulator;
    QMutex _mutex;
};

#endif // EMULATORWORKER_H
