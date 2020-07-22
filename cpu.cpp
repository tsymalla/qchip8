#include "cpu.h"
#include <QFile>
#include <QMutexLocker>

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
                const auto reg = _baseRegisters[_opcode & 0x00F0];
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
                const auto reg = _baseRegisters[_opcode & 0x00F0];
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
                _baseRegisters[(_opcode & 0x0F00) >> 8] = (_opcode & 0x00F0);
                _stepProgramCounter();

                break;
            }
            case 0x7000:
            {
                _baseRegisters[(_opcode & 0x0F00) >> 8] += (_opcode & 0x00F0);
                _stepProgramCounter();

                break;
            }
            default:
                break;
        }
    }
}
