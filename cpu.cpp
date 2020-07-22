#include "cpu.h"
#include <utility>
#include <QFile>
#ifdef QT_DEBUG
    #include <QDebug>
#endif

namespace Chip8
{
    CPU::CPU(QString filename): _filename(std::move(filename)), _isRunning(false), _canRefreshScreen(false)
    {
    }

    void CPU::loadROM()
    {
        QFile fileDescriptor(_filename);
        if (!fileDescriptor.open(QIODevice::ReadOnly))
        {
            emit romLoadingFailed();
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
        _clearStack();
        _stackPointer = { 0x0000 };
        _framebuffer.fill({ 0x00 });
        _keyStatus.fill({ 0x00 });

        _isRunning = true;
        emit romLoaded();
    }

    void CPU::reset()
    {
        _isRunning = false;
    }

    void CPU::run()
    {
        while (isRunning())
        {
            _cycle();

            // if the draw flag is set, emit an signal to update the parent with the current framebuffer
            if (_canRefreshScreen)
            {
                emit refreshScreen(_framebuffer);
                _canRefreshScreen = false;
            }
        }
    }

    bool CPU::isRunning() const
    {
        return _isRunning;
    }

    void CPU::_cycle()
    {
        _opcode = _memory[_programCounter] << 8 | _memory[_programCounter + 1];
        #ifdef QT_DEBUG
            qDebug() << _opcode;
        #endif
    }

    void CPU::_clearStack()
    {
        while (!_stack.empty())
        {
            _stack.pop();
        }
    }
}
