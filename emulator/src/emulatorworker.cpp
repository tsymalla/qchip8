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

void EmulatorWorker::keyDown(int key)
{
	QMutexLocker locker(&_mutex);
	_emulator.keyDown(key);
}

void EmulatorWorker::keyUp(int key)
{
	QMutexLocker locker(&_mutex);
	_emulator.keyUp(key);
}

void EmulatorWorker::onRefreshScreen(Chip8::FrameBuffer framebuffer)
{
	// forward message
	emit refreshScreen(framebuffer);
}

void EmulatorWorker::onRunEmulation()
{
	_emulator.reset();
	_emulator.run();

	emit finishedEmulation();
}

void EmulatorWorker::onStopEmulation()
{
	_mutex.lock();
	_emulator.stop();
	_mutex.unlock();

	emit finishedEmulation();
}

bool EmulatorWorker::isRunning() const
{
	return _emulator.isRunning();
}
