#include "emulatorworker.h"
#include <utility>

EmulatorWorker::EmulatorWorker(QString filename, QObject *parent): QObject(parent)
{
    _emulator = new Chip8::CPU(std::move(filename), this);
    _emulator->loadROM();

    connect(_emulator, &Chip8::CPU::refreshScreen, this, &EmulatorWorker::onRefreshScreen);
}

void EmulatorWorker::setROM(QString filename)
{
    _emulator->setROM(std::move(filename));
    _emulator->loadROM();
}

void EmulatorWorker::onRefreshScreen(Chip8::FrameBuffer framebuffer)
{
    // forward message
    emit refreshScreen(framebuffer);
}

void EmulatorWorker::onRunEmulation()
{
    QMutexLocker locker(&_mutex);

    _emulator->run();

    emit finishedEmulation();
}

void EmulatorWorker::stopEmulation()
{
    _emulator->stop();

    emit finishedEmulation();
}
