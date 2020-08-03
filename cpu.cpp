#include "cpu.h"
#include <QFile>
#include <QMutexLocker>
#include <QDebug>
#include <QRandomGenerator>

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
        //qDebug() << _programCounter << " " << QString::number(_opcode, 16);

        const auto x = _opcode & 0x0F00;
        const size_t registerX = x >> 8;

        const auto y = _opcode & 0x00F0;
        const auto registerY = y >> 4;

        const auto nnn = _opcode & 0x0FFF;
        const auto nn = _opcode & 0x00FF;
        const auto n = _opcode & 0x000F;

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
                const auto reg = _registerSet.getRegisterValue(registerX);
                if (reg == nn)
                {
                    _stepProgramCounterByte();
                }

                _stepProgramCounterByte();

                break;
            }
            case 0x4000:
            {
                const auto reg = _registerSet.getRegisterValue(registerX);
                if (reg != nn)
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
                switch (_opcode & 0x000F)
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

                        if (_registerSet.getRegisterValue(registerY) > (0xFF - _registerSet.getRegisterValue(registerX)))
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

                        _registerSet.setRegisterValue(registerX, _registerSet.getRegisterValue(registerY) - _registerSet.getRegisterValue(registerX));

                        if (_registerSet.getRegisterValue(registerX) > _registerSet.getRegisterValue(registerY))
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
                _registerSet.setRegisterValue(registerX, random & (_opcode & nn));

                _stepProgramCounterByte();
                break;
            }
            case 0xD000:
            {
                const auto posX = static_cast<Word>(_registerSet.getRegisterValue(registerX));
                const auto posY = static_cast<Word>(_registerSet.getRegisterValue(registerY));
                const auto height = n;
                const auto addressRegister = _registerSet.getAddressRegister();

                _registerSet.setRegisterValue(0xF, 0);

                for (size_t vy = 0; vy < height; ++vy)
                {
                    const unsigned short pixels = _memory[addressRegister + posY];

                    for (size_t vx = 0; vx < SPRITE_WIDTH; ++vx)
                    {
                        const size_t index = (posX + vx + ((posY + vy) * DISPLAY_WIDTH));
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
                        _stepProgramCounterByte();
                        break;
                    }
                    case 0x00A1:
                    {
                        // TODO
                        _stepProgramCounterByte();
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
                        _registerSet.setRegisterValue(registerX, _registerSet.getDelayTimer());
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
                        if (_registerSet.getAddressRegister() + _registerSet.getRegisterValue(registerX) > 0xFFF)
                        {
                            _registerSet.setRegisterValue(0xF, 1);
                        }
                        else
                        {
                            _registerSet.setRegisterValue(0xF, 0);
                        }

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
                        _memory[addressRegister + 2] = reg % 10;

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
                //qDebug().noquote() << "Invalid opcode: " << QString::number(_opcode, 16);
                break;
        }

        if (_registerSet.getDelayTimer() > 0)
        {
            _registerSet.decDelayTimer();
        }

        if (_registerSet.getSoundTimer() > 0)
        {
            _registerSet.decSoundTimer();
        }
    }
}
