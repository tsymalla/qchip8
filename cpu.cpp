#include "cpu.h"
#include <QFile>
#include <QMutexLocker>
#include <QDebug>

namespace Chip8
{
    CPU::CPU(QString filename, QObject *parent): QObject(parent), _filename(std::move(filename)), _isRunning(false), _canRefreshScreen(false)
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
        _baseRegisters.fill(0x00);
        _addressRegister = { 0x0000 };
        _programCounter = { 0x0200 };
        _delayTimer = { 0x00 };
        _soundTimer = { 0x00 };
        _opcode = { 0x0000 };
        _stack.fill(0x0000);
        _stackPointer = { 0x0000 };
        _framebuffer.fill({ 0x00 });
        _keyStatus.fill({ 0x00 });
    }

    void CPU::reset()
    {
        _isRunning = false;
    }

    void CPU::run()
    {
        QMutexLocker locker(&_mutex);

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
        }
    }

    void CPU::stop()
    {
        QMutexLocker locker(&_mutex);

        _isRunning = false;
    }

    bool CPU::isRunning() const
    {
        return _isRunning;
    }

    void CPU::_stepProgramCounter()
    {
        _programCounter += 2;
    }

    void CPU::_cycle()
    {
        _opcode = _memory.readWord(_programCounter);

        switch (_opcode & 0xF000)
        {
            case 0x0000:
            {
                switch (_opcode & 0x000F)
                {
                    case 0x0000:
                    {
                        _framebuffer.fill(0x00);
                        _canRefreshScreen = true;
                        _stepProgramCounter();
                        break;
                    }
                    case 0x000E:
                    {
                        _programCounter = _stack[_stackPointer];
                        --_stackPointer;
                        _stepProgramCounter();
                        break;
                    }
                }

                break;
            }
            case 0x1000:
            {
                _programCounter = _opcode & 0x0FFF;
                break;
            }
            case 0x2000:
            {
                ++_stackPointer;
                _stack[_stackPointer] = _programCounter;
                _programCounter = _opcode & 0x0FFF;
                break;
            }
            case 0x3000:
            {
                const auto reg = _baseRegisters[_opcode & 0x0F00] >> 8;
                if ((_opcode & 0x00FF) == reg)
                {
                    _stepProgramCounter();
                    _stepProgramCounter();
                }
                else
                {
                    _stepProgramCounter();
                }

                break;
            }
            case 0x4000:
            {
                const auto reg = _baseRegisters[_opcode & 0x0F00] >> 8;
                if ((_opcode & 0x00FF) != reg)
                {
                    _stepProgramCounter();
                    _stepProgramCounter();
                }
                else
                {
                    _stepProgramCounter();
                }

                break;
            }
            case 0x5000:
            {
                const auto reg = _baseRegisters[_opcode & 0x00F0];
                const auto reg2 = _baseRegisters[(_opcode & 0x0F00) >> 8];

                if (reg != reg2)
                {
                    _stepProgramCounter();
                    _stepProgramCounter();
                }
                else
                {
                    _stepProgramCounter();
                }

                break;
            }
            case 0x6000:
            {
                _baseRegisters[(_opcode & 0x0F00) >> 8] = (_opcode & 0x00FF);
                _stepProgramCounter();

                break;
            }
            case 0x7000:
            {
                _baseRegisters[(_opcode & 0x0F00) >> 8] += (_opcode & 0x00F0);
                _stepProgramCounter();

                break;
            }
            case 0x8000:
            {
                switch (_opcode & 0x000F)
                {
                    case 0x0000:
                    {
                        _baseRegisters[(_opcode & 0x0F00) >> 8] = _opcode & 0x00F0;
                        _stepProgramCounter();
                        break;
                    }
                    case 0x0001:
                    {
                        _baseRegisters[(_opcode & 0x0F00) >> 8] = (_opcode & 0x0F00) | (_opcode & 0x00F0);
                        _stepProgramCounter();
                        break;
                    }
                    case 0x0002:
                    {
                        _baseRegisters[(_opcode & 0x0F00) >> 8] = (_opcode & 0x0F00) & (_opcode & 0x00F0);
                        _stepProgramCounter();
                        break;
                    }
                    case 0x0003:
                    {
                        _baseRegisters[(_opcode & 0x0F00) >> 8] = (_opcode & 0x0F00) ^ (_opcode & 0x00F0);
                        _stepProgramCounter();
                        break;
                    }
                    case 0x0004:
                    {
                        _baseRegisters[(_opcode & 0x0F00) >> 8] += _baseRegisters[(_opcode & 0x00F0) >> 4];

                        if (_baseRegisters[(_opcode & 0x00F0) >> 4] > (0xFF - _baseRegisters[(_opcode & 0x0F00) >> 8]))
                        {
                            _baseRegisters[0xF] = 1;
                        }
                        else
                        {
                            _baseRegisters[0xF] = 0;
                        }

                        _stepProgramCounter();
                        break;
                    }
                    case 0x0005:
                    {
                        _baseRegisters[(_opcode & 0x0F00) >> 8] -= _baseRegisters[(_opcode & 0x00F0) >> 4];

                        if (_baseRegisters[(_opcode & 0x00F0) >> 4] > _baseRegisters[(_opcode & 0x0F00) >> 8])
                        {
                            _baseRegisters[0xF] = 1;
                        }
                        else
                        {
                            _baseRegisters[0xF] = 0;
                        }

                        _stepProgramCounter();
                        break;
                    }
                    case 0x0006:
                    {
                        _baseRegisters[0xF] = _baseRegisters[(_opcode & 0x0F00) >> 8] & 0x01;
                        _baseRegisters[(_opcode & 0x0F00) >> 8] >>= 1;

                        _stepProgramCounter();
                        break;
                    }
                    case 0x0007:
                    {
                        if (_baseRegisters[(_opcode & 0x0F00) >> 8] > _baseRegisters[(_opcode & 0x00F0) >> 4])
                        {
                            _baseRegisters[0xF] = 1;
                        }
                        else
                        {
                            _baseRegisters[0xF] = 0;
                        }

                        _baseRegisters[(_opcode & 0x0F00) >> 8] = _baseRegisters[(_opcode & 0x00F0) >> 4] - _baseRegisters[(_opcode & 0x0F00) >> 8];

                        _stepProgramCounter();
                        break;
                    }
                    case 0x000E:
                    {
                        _baseRegisters[0xF] = _baseRegisters[(_opcode & 0x0F00) >> 8] >> 7;
                        _baseRegisters[(_opcode & 0x0F00) >> 8] <<= 1;

                        _stepProgramCounter();
                        break;
                    }
                }
            }
        case 0x9000:
        {
            if (_baseRegisters[(_opcode & 0x0F00) >> 8] != _baseRegisters[(_opcode & 0x00F0) >> 4])
            {
                _stepProgramCounter();
                _stepProgramCounter();
            }
            else
            {
                _stepProgramCounter();
            }

            break;
        }
        case 0xA000:
        {
            _addressRegister = _opcode & 0x0FFF;
            _stepProgramCounter();

            break;
        }
        case 0xB000:
        {
            _programCounter = (_opcode & 0x0FFF) + _baseRegisters[0];
            break;
        }
        case 0xC000:
        {
            // TODO
            _stepProgramCounter();
            break;
        }
        case 0xD000:
        {
            unsigned short pixels = 0;
            unsigned short x = _baseRegisters[(_opcode & 0x0F00) >> 8];
            unsigned short y = _baseRegisters[(_opcode & 0x00F0) >> 4];
            unsigned short height = _opcode & 0x000F;

            _baseRegisters[0xF] = 0;

            for (size_t vy = 0; vy < height; ++vy)
            {
                pixels = _memory[_addressRegister + y];

                for (size_t vx = 0; vx < SPRITE_WIDTH; ++vx)
                {
                    if ((pixels & (0x80 >> vx)) != 0)
                    {
                        _baseRegisters[0xF] = 1;
                    }

                    _framebuffer[x + vx + ((y + vy) * DISPLAY_HEIGHT)] ^= 1;
                }
            }

            _canRefreshScreen = true;
            _stepProgramCounter();
        }
        default:
            qDebug().noquote() << "Invalid opcode: " << QString::number(_opcode, 16);
            break;
        }
    }
}
