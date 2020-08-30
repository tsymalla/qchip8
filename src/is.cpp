#include "is.h"
#include <QDebug>
#include <QRandomGenerator>

namespace Chip8
{
    IS::IS(Word& programCounter, RegisterSet& registerSet, Memory& memory, FrameBuffer& framebuffer, KeyBuffer& keybuffer, QObject* parent) :
		QObject(parent),
		_programCounter(programCounter),
		_registerSet(registerSet),
		_memory(memory),
        _framebuffer(framebuffer),
        _keybuffer(keybuffer)
	{
	}

	bool IS::step(const Word& opcode)
	{
		bool refreshFlag = false;

		const Byte registerX = (opcode & 0x0F00) >> 8;
		const Byte registerY = (opcode & 0x00F0) >> 4;
		const Word nnn = opcode & 0x0FFF;
		const Byte nn = opcode & 0x00FF;
		const Byte n = opcode & 0x000F;

		qDebug() << QString::number(_programCounter, 16) << "\t" << QString::number(opcode, 16) << "\t"
			<< registerX
			<< "\t"
			<< registerY
			<< "\t"
			<< QString::number(nnn, 16)
			<< "\t"
			<< QString::number(nn, 16)
			<< "\t"
			<< QString::number(n, 16);

		switch (opcode & 0xF000)
		{
		case 0x0000:
		{
			switch (opcode & 0x000F)
			{
			case 0x0000:
			{
				_framebuffer.fill(0x00);
				refreshFlag = true;
				_stepProgramCounterByte();

				break;
			}
			case 0x000E:
			{
				_programCounter = _registerSet.popStack();
				_stepProgramCounterByte();

				break;
			}
			}

			break;
		}
		case 0x1000:
		{
			_programCounter = nnn;

			break;
		}
		case 0x2000:
		{
			_registerSet.pushStack(_programCounter);
			_programCounter = nnn;

			break;
		}
		case 0x3000:
		{
			const auto regX = _registerSet.getRegisterValue(registerX);
			if (regX == nn)
			{
				_stepProgramCounterByte();
			}

			_stepProgramCounterByte();

			break;
		}
		case 0x4000:
		{
			const auto regX = _registerSet.getRegisterValue(registerX);
			if (regX != nn)
			{
				_stepProgramCounterByte();
			}

			_stepProgramCounterByte();

			break;
		}
		case 0x5000:
		{
			const auto regX = _registerSet.getRegisterValue(registerX);
			const auto regY = _registerSet.getRegisterValue(registerY);

			if (regX == regY)
			{
				_stepProgramCounterByte();
			}

			_stepProgramCounterByte();

			break;
		}
		case 0x6000:
		{
			_registerSet.setRegisterValue(registerX, nn);
			_stepProgramCounterByte();

			break;
		}
		case 0x7000:
		{
			_registerSet.addRegisterValue(registerX, nn);
			_stepProgramCounterByte();

			break;
		}
		case 0x8000:
		{
			switch (opcode & 0x000F)
			{
			case 0x0000:
			{
				_registerSet.setRegisterValue(registerX, _registerSet.getRegisterValue(registerY));
				_stepProgramCounterByte();

				break;
			}
			case 0x0001:
			{
				_registerSet.setRegisterValue(registerX, _registerSet.getRegisterValue(registerX) | _registerSet.getRegisterValue(registerY));
				_stepProgramCounterByte();

				break;
			}
			case 0x0002:
			{
				_registerSet.setRegisterValue(registerX, _registerSet.getRegisterValue(registerX) & _registerSet.getRegisterValue(registerY));
				_stepProgramCounterByte();

				break;
			}
			case 0x0003:
			{
				_registerSet.setRegisterValue(registerX, _registerSet.getRegisterValue(registerX) ^ _registerSet.getRegisterValue(registerY));
				_stepProgramCounterByte();

				break;
			}
			case 0x0004:
			{
				_registerSet.addRegisterValue(registerX, _registerSet.getRegisterValue(registerY));

				if (_registerSet.getRegisterValue(registerY) + _registerSet.getRegisterValue(registerX) > 0xFF)
				{
					_registerSet.setRegisterValue(0xF, 1);
				}
				else
				{
					_registerSet.setRegisterValue(0xF, 0);
				}

				_stepProgramCounterByte();
				break;
			}
			case 0x0005:
			{
				_registerSet.subRegisterValue(registerX, _registerSet.getRegisterValue(registerY));

				if (_registerSet.getRegisterValue(registerY) > _registerSet.getRegisterValue(registerX))
				{
					_registerSet.setRegisterValue(0xF, 0);
				}
				else
				{
					_registerSet.setRegisterValue(0xF, 1);
				}

				_stepProgramCounterByte();
				break;
			}
			case 0x0006:
			{
				_registerSet.setRegisterValue(0xF, _registerSet.getRegisterValue(registerX) & 0x01);
				_registerSet.shrRegisterValue(registerX, 1);

				_stepProgramCounterByte();
				break;
			}
			case 0x0007:
			{
				if (_registerSet.getRegisterValue(registerX) > _registerSet.getRegisterValue(registerY))
				{
					_registerSet.setRegisterValue(0xF, 0);
				}
				else
				{
					_registerSet.setRegisterValue(0xF, 1);
				}

				_registerSet.setRegisterValue(registerX, _registerSet.getRegisterValue(registerY) - _registerSet.getRegisterValue(registerX));
				_stepProgramCounterByte();

				break;
			}
			case 0x000E:
			{
				_registerSet.setRegisterValue(0xF, _registerSet.getRegisterValue(registerX) >> 7);
				_registerSet.shlRegisterValue(registerX, 1);

				_stepProgramCounterByte();
				break;
			}
			}

			break;
		}
		case 0x9000:
		{
			if (_registerSet.getRegisterValue(registerX) != _registerSet.getRegisterValue(registerY))
			{
				_stepProgramCounterByte();
			}

			_stepProgramCounterByte();

			break;
		}
		case 0xA000:
		{
			_registerSet.setAddressRegister(nnn);
			_stepProgramCounterByte();

			break;
		}
		case 0xB000:
		{
			_programCounter = nnn + _registerSet.getRegisterValue(0);
			break;
		}
		case 0xC000:
		{
			const auto random = static_cast<Byte>(QRandomGenerator::global()->bounded(0x00, 0xFF));
			_registerSet.setRegisterValue(registerX, random & nn);

			_stepProgramCounterByte();
			break;
		}
		case 0xD000:
		{
			const auto posX = _registerSet.getRegisterValue(registerX);
			const auto posY = _registerSet.getRegisterValue(registerY);
			const auto height = n;
			const auto addressRegister = _registerSet.getAddressRegister();

			_registerSet.setRegisterValue(0xF, 0);

			for (size_t vy = 0; vy < height; ++vy)
			{
				const auto pixelIndex = addressRegister + vy;
				const unsigned short pixel = _memory[pixelIndex];

				for (size_t vx = 0; vx < SPRITE_WIDTH; ++vx)
				{
					size_t index = (posX + vx + ((posY + vy) * DISPLAY_WIDTH));
					if ((pixel & (0x80 >> vx)) != 0)
					{
						// implement wraparound
						if (index > DISPLAY_SIZE)
						{
							index %= DISPLAY_SIZE;
						}

						if (_framebuffer[index] == 1)
						{
							_registerSet.setRegisterValue(0xF, 1);
						}

						_framebuffer[index] ^= 1;
					}
				}
			}

			refreshFlag = true;
			_stepProgramCounterByte();

			break;
		}
		case 0xE000:
		{
            switch (nn)
			{
			case 0x009E:
            {
                const auto regX = _registerSet.getRegisterValue(registerX);
                if (_keybuffer[regX])
                {
                    // key was pressed
                    _stepProgramCounterByte();
                }

				_stepProgramCounterByte();
				break;
			}
			case 0x00A1:
            {
                const auto regX = _registerSet.getRegisterValue(registerX);
                if (!_keybuffer[regX])
                {
                    // key was pressed
                    _stepProgramCounterByte();
                }

				_stepProgramCounterByte();
				break;
			}
			}

			break;
		}
		case 0xF000:
		{
            switch (nn)
			{
			case 0x0007:
			{
				_registerSet.setRegisterValue(registerX, _registerSet.getDelayTimer());
				_stepProgramCounterByte();

				break;
			}
			case 0x000A:
            {
                bool isPressed = false;

                for (size_t i = 0; i < KEY_COUNT; ++i)
                {
                    if (_keybuffer[i])
                    {
                        _registerSet.setRegisterValue(registerX, i);
                        isPressed = true;
                    }
                }

                if (!isPressed)
                {
                    break;
                }

				_stepProgramCounterByte();

				break;
			}
			case 0x0015:
			{
				_registerSet.setDelayTimer(_registerSet.getRegisterValue(registerX));
				_stepProgramCounterByte();

				break;
			}
			case 0x0018:
			{
				_registerSet.setSoundTimer(_registerSet.getRegisterValue(registerX));
				_stepProgramCounterByte();

				break;
			}
			case 0x001E:
			{
				_registerSet.incAddressRegister(_registerSet.getRegisterValue(registerX));
				_stepProgramCounterByte();

				break;
			}
			case 0x0029:
			{
				_registerSet.setAddressRegister(_registerSet.getRegisterValue(registerX) * 0x5);
				_stepProgramCounterByte();

				break;
			}
			case 0x0033:
			{
				const auto reg = _registerSet.getRegisterValue(registerX);
				const auto addressRegister = _registerSet.getAddressRegister();

				_memory[addressRegister] = reg / 100;
				_memory[addressRegister + 1] = (reg / 10) % 10;
				_memory[addressRegister + 2] = reg % 100 % 10;

				_stepProgramCounterByte();
				break;
			}
			case 0x0055:
			{
				const auto addressRegister = _registerSet.getAddressRegister();

				for (size_t i = 0; i <= registerX; ++i)
				{
					_memory[addressRegister + i] = _registerSet.getRegisterValue(i);
				}

				_registerSet.incAddressRegister(static_cast<Word>(registerX) + 1);
				_stepProgramCounterByte();

				break;
			}
			case 0x0065:
			{
				const auto addressRegister = _registerSet.getAddressRegister();

				for (size_t i = 0; i <= registerX; ++i)
				{
					_registerSet.setRegisterValue(i, _memory[addressRegister + i]);
				}

				_registerSet.incAddressRegister(static_cast<Word>(registerX) + 1);
				_stepProgramCounterByte();

				break;
			}
			}

			break;
		}
		default:
			//qDebug().noquote() << "Invalid opcode: " << QString::number(opcode, 16);
			break;
		}

		return refreshFlag;
	}

	void IS::_stepProgramCounterByte()
	{
		_programCounter += 2;
	}
}
