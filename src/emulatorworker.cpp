#include "emulatorworker.h"
#include <utility>

EmulatorWorker::EmulatorWorker(QObject* parent) : QObject(parent)
{
}

void EmulatorWorker::setROM(QString filename)
{
    _emulator.reset();
    _emulator.setROM(std::move(filename));
    _emulator.loadROM();
    connect(&_emulator, &Chip8::CPU::refreshScreen, this, &EmulatorWorker::onRefreshScreen);
}

void EmulatorWorker::onRefreshScreen(Chip8::FrameBuffer framebuffer)
{
    // forward message
    emit refreshScreen(framebuffer);
}

void EmulatorWorker::onRunEmulation()
{
    _emulator.run();

    emit finishedEmulation();
}

void EmulatorWorker::stopEmulation()
{
    _mutex.lock();
    _emulator.stop();
    _mutex.unlock();

    emit finishedEmulation();
}