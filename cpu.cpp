#include "cpu.h"
#include <QFile>
#include <QMutexLocker>
#include <QDebug>
#include <QRandomGenerator>

namespace Chip8
{
    CPU::CPU(QObject *parent): QObject(parent), _isRunning(false), _canRefreshScreen(false)
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
        _keyStatus.fill({ 0x00 });
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

    void CPU::_stepProgramCounterByte()
    {
        _programCounter += 2;
    }

    void CPU::_stepProgramCounterWord()
    {
        _programCounter += 4;
    }

    void CPU::_cycle()
    {
        _opcode = _memory.readWord(_programCounter);

        size_t registerIndex = (_opcode & 0x0F00) >> 8;

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
                _programCounter = _opcode & 0x0FFF;
                break;
            }
            case 0x2000:
            {
                _registerSet.pushStack(_programCounter);
                _programCounter = _opcode & 0x0FFF;
                break;
            }
            case 0x3000:
            {
                const auto reg = _registerSet.getRegisterValue(registerIndex);
                if ((_opcode & 0x00FF) == reg)
                {
                    _stepProgramCounterWord();
                }
                else
                {
                    _stepProgramCounterByte();
                }

                break;
            }
            case 0x4000:
            {
                const auto reg = _registerSet.getRegisterValue(registerIndex);
                if ((_opcode & 0x00FF) != reg)
                {
                    _stepProgramCounterWord();
                }
                else
                {
                    _stepProgramCounterByte();
                }

                break;
            }
            case 0x5000:
            {
                const auto reg = _registerSet.getRegisterValue(_opcode & 0x00F0);
                const auto reg2 = _registerSet.getRegisterValue(registerIndex);

                if (reg == reg2)
                {
                    _stepProgramCounterWord();
                }
                else
                {
                    _stepProgramCounterByte();
                }

                break;
            }
            case 0x6000:
            {
                _registerSet.setRegisterValue(registerIndex, (_opcode & 0x00FF));
                _stepProgramCounterByte();

                break;
            }
            case 0x7000:
            {
                _registerSet.addRegisterValue(registerIndex, (_opcode & 0x00FF));
                _stepProgramCounterByte();

                break;
            }
            case 0x8000:
            {
                switch (_opcode & 0x000F)
                {
                    case 0x0000:
                    {
                        _registerSet.setRegisterValue(registerIndex, (_opcode & 0x00F0));
                        _stepProgramCounterByte();

                        break;
                    }
                    case 0x0001:
                    {
                        _registerSet.setRegisterValue(registerIndex, (_opcode & 0x0F00) | (_opcode & 0x00F0));
                        _stepProgramCounterByte();

                        break;
                    }
                    case 0x0002:
                    {
                        _registerSet.setRegisterValue(registerIndex, (_opcode & 0x0F00) & (_opcode & 0x00F0));
                        _stepProgramCounterByte();

                        break;
                    }
                    case 0x0003:
                    {
                        _registerSet.setRegisterValue(registerIndex, (_opcode & 0x0F00) ^ (_opcode & 0x00F0));
                        _stepProgramCounterByte();

                        break;
                    }
                    case 0x0004:
                    {
                        _registerSet.addRegisterValue(registerIndex, _registerSet.getRegisterValue((_opcode & 0x00F0) >> 4));

                        if (_registerSet.getRegisterValue((_opcode & 0x00F0) >> 4) > (0xFF - _registerSet.getRegisterValue((_opcode & 0x0F00) >> 8)))
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
                        _registerSet.subRegisterValue(registerIndex, _registerSet.getRegisterValue((_opcode & 0x00F0) >> 4));

                        if (_registerSet.getRegisterValue((_opcode & 0x00F0) >> 4) > _registerSet.getRegisterValue(registerIndex))
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
                        _registerSet.setRegisterValue(0xF, _registerSet.getRegisterValue((_opcode & 0x0F00) >> 8) & 0x01);
                        _registerSet.shrRegisterValue(registerIndex, 1);

                        _stepProgramCounterByte();
                        break;
                    }
                    case 0x0007:
                    {
                        if (_registerSet.getRegisterValue(registerIndex) > _registerSet.getRegisterValue((_opcode & 0x00F0) >> 4))
                        {
                            _registerSet.setRegisterValue(0xF, 0);
                        }
                        else
                        {
                            _registerSet.setRegisterValue(0xF, 1);
                        }

                        _registerSet.setRegisterValue(registerIndex, _registerSet.getRegisterValue((_opcode & 0x00F0) >> 4) - _registerSet.getRegisterValue((_opcode & 0x0F00) >> 8));

                        _stepProgramCounterByte();
                        break;
                    }
                    case 0x000E:
                    {
                        _registerSet.setRegisterValue(0xF, _registerSet.getRegisterValue((_opcode & 0x0F00) >> 8) >> 7);
                        _registerSet.shlRegisterValue(registerIndex, 1);

                        _stepProgramCounterByte();
                        break;
                    }
                }
            }
        case 0x9000:
        {
            if (_registerSet.getRegisterValue(registerIndex) != _registerSet.getRegisterValue((_opcode & 0x00F0) >> 4))
            {
                _stepProgramCounterWord();
            }
            else
            {
                _stepProgramCounterByte();
            }

            break;
        }
        case 0xA000:
        {
            _registerSet.setAddressRegister(_opcode & 0x0FFF);
            _stepProgramCounterByte();

            break;
        }
        case 0xB000:
        {
            _programCounter = (_opcode & 0x0FFF) + _registerSet.getRegisterValue(0);
            break;
        }
        case 0xC000:
        {
            auto random = static_cast<Byte>(QRandomGenerator::global()->bounded(0x00, 0xFF));
            _registerSet.setRegisterValue(registerIndex, random & (_opcode & 0x00FF));

            _stepProgramCounterByte();
            break;
        }
        case 0xD000:
        {
            unsigned short pixels = 0;
            unsigned short x = _registerSet.getRegisterValue(registerIndex);
            unsigned short y = _registerSet.getRegisterValue((_opcode & 0x00F0) >> 4);
            unsigned short height = _opcode & 0x000F;
            auto addressRegister = _registerSet.getAddressRegister();

            _registerSet.setRegisterValue(0xF, 0);

            for (size_t vy = 0; vy < height; ++vy)
            {
                pixels = _memory[addressRegister + y];

                for (size_t vx = 0; vx < SPRITE_WIDTH; ++vx)
                {
                    const size_t index = (x + vx + ((y + vy) * DISPLAY_WIDTH));
                    if ((pixels & (0x80 >> vx)) != 0)
                    {
                        if (_framebuffer[index] == 1)
                        {
                            _registerSet.setRegisterValue(0xF, 1);
                        }

                        _framebuffer[index] ^= 1;
                    }
                }
            }

            _canRefreshScreen = true;
            _stepProgramCounterByte();
        }
        case 0xE000:
        {
            switch (_opcode & 0x00FF)
            {
                case 0x009E:
                {
                    // TODO
                    break;
                }
                case 0x00A1:
                {
                    // TODO
                    break;
                }
            }
        }
        case 0xF000:
        {
            switch (_opcode & 0x00FF)
            {
                case 0x0007:
                {
                    _registerSet.setRegisterValue(registerIndex, _registerSet.getDelayTimer());
                    _stepProgramCounterByte();

                    break;
                }
                case 0x000A:
                {
                    // TODO
                    _stepProgramCounterByte();

                    break;
                }
                case 0x0015:
                {
                    _registerSet.setDelayTimer(_registerSet.getRegisterValue(registerIndex));
                    _stepProgramCounterByte();

                    break;
                }
                case 0x0018:
                {
                    // TODO
                    _stepProgramCounterByte();

                    break;
                }
                case 0x001E:
                {
                    _registerSet.setAddressRegister(_registerSet.getRegisterValue(registerIndex));
                    _stepProgramCounterByte();

                    break;
                }
                case 0x0029:
                {
                    _registerSet.setAddressRegister(_registerSet.getRegisterValue(registerIndex) * 5);
                    _stepProgramCounterByte();

                    break;
                }
                case 0x0033:
                {
                    const auto reg = _registerSet.getRegisterValue(registerIndex);
                    const auto addressRegister = _registerSet.getAddressRegister();

                    _memory[addressRegister] = reg / 100;
                    _memory[addressRegister + 1] = (reg / 10) % 10;
                    _memory[addressRegister + 2] = (reg % 100) % 10;

                    _stepProgramCounterByte();
                    break;
                }
                case 0x0055:
                {
                    const size_t index = (_opcode & 0x0F00) >> 8;
                    const auto addressRegister = _registerSet.getAddressRegister();

                    for (size_t i = 0; i <= index; ++i)
                    {
                        _memory[addressRegister + i] = _registerSet.getRegisterValue(i);
                    }

                    _registerSet.incAddressRegister(index + 1);
                    _stepProgramCounterByte();

                    break;
                }
                case 0x0065:
                {
                    const size_t index = (_opcode & 0x0F00) >> 8;
                    const auto addressRegister = _registerSet.getAddressRegister();

                    for (size_t i = 0; i <= index; ++i)
                    {
                        _registerSet.setRegisterValue(i, _memory[addressRegister + i]);
                    }

                    _registerSet.incAddressRegister(index + 1);
                    _stepProgramCounterByte();

                    break;
                }
            }

            break;
        }
        default:
            qDebug().noquote() << "Invalid opcode: " << QString::number(_opcode, 16);
            break;
        }

        if (_registerSet.getDelayTimer() > 0)
        {
            _registerSet.decDelayTimer();
        }
    }
}
