#include "cpu.h"
#include <QFile>
#include <QThread>

namespace Chip8
{
	CPU::CPU(QObject* parent) : QObject(parent), _isRunning(false), _canRefreshScreen(false)
	{
	}

	void CPU::setROM(QString filename)
	{
		_filename = std::move(filename);
	}

	void CPU::loadROM()
	{
		QFile fileDescriptor(_filename);
		if (!fileDescriptor.open(QIODevice::ReadOnly))
		{
			return;
		}

		QByteArray romData = fileDescriptor.readAll();
		_memory.resetMemory();
		_memory = romData;

		_isRunning = false;
		_canRefreshScreen = false;
		_registerSet.reset();
		_programCounter = { 0x0200 };
		_opcode = { 0x0000 };
		_framebuffer.fill({ 0x00 });
		_keyStatus.fill({ false });

        _is = new IS(_programCounter, _registerSet, _memory, _framebuffer, _keyStatus, this);
	}

	void CPU::reset()
	{
		_isRunning = false;
	}

	void CPU::run()
	{
		_isRunning = true;
		_canRefreshScreen = true;

		while (isRunning())
		{
			_cycle();

			// if the draw flag is set, emit an signal to update the parent with the current framebuffer
			if (_canRefreshScreen)
			{
				_canRefreshScreen = false;
				emit refreshScreen(_framebuffer);
			}

			QThread::usleep(1200);
		}
	}

	void CPU::stop()
	{
		_isRunning = false;
	}

	bool CPU::isRunning() const
	{
		return _isRunning;
	}

	void CPU::keyDown(int key)
	{
		if (KEY_MAP.count(key) == 0)
		{
			return;
		}

		_keyStatus[KEY_MAP.at(key)] = true;
	}

	void CPU::keyUp(int key)
	{
		if (KEY_MAP.count(key) == 0)
		{
			return;
		}

		_keyStatus[KEY_MAP.at(key)] = false;
	}

	void CPU::_decode()
	{
		_opcode = _memory.readWord(_programCounter);
	}

	void CPU::_execute()
	{
		_canRefreshScreen = _is->step(_opcode);

		if (_registerSet.getDelayTimer() > 0)
		{
			_registerSet.decDelayTimer();
		}

		if (_registerSet.getSoundTimer() > 0)
		{
			_registerSet.decSoundTimer();
		}
	}

	void CPU::_cycle()
	{
		_decode();
		_execute();
	}
}
