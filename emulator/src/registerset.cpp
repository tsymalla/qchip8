#include "registerset.h"

namespace Chip8
{
	void RegisterSet::reset()
	{
		_baseRegisters.fill(0x00);
		_addressRegister = { 0x0000 };
		_delayTimer = { 0x00 };
		_soundTimer = { 0x00 };
		_stack.fill(0x0000);
		_stackPointer = { 0x0000 };
	}

	void RegisterSet::pushStack(Word value)
	{
		++_stackPointer;
		_stack[_stackPointer] = value;
	}

	Word RegisterSet::popStack()
	{
		const auto value = _stack[_stackPointer];
		--_stackPointer;

		return value;
	}

	void RegisterSet::setRegisterValue(size_t index, Byte value)
	{
		assert(index >= 0 && index < REGISTER_COUNT);
		_baseRegisters[index] = value;
	}

	void RegisterSet::addRegisterValue(size_t index, Byte value)
	{
		assert(index >= 0 && index < REGISTER_COUNT);
		_baseRegisters[index] += value;
	}

	void RegisterSet::subRegisterValue(size_t index, Byte value)
	{
		assert(index >= 0 && index < REGISTER_COUNT);
		_baseRegisters[index] -= value;
	}

	void RegisterSet::shrRegisterValue(size_t index, size_t count)
	{
		assert(index >= 0 && index < REGISTER_COUNT);
		_baseRegisters[index] >>= count;
	}

	void RegisterSet::shlRegisterValue(size_t index, size_t count)
	{
		assert(index >= 0 && index < REGISTER_COUNT);
		_baseRegisters[index] <<= count;
	}

	Byte RegisterSet::getRegisterValue(size_t index) const
	{
		assert(index >= 0 && index < REGISTER_COUNT);
		return _baseRegisters[index];
	}

	void RegisterSet::setAddressRegister(Word value)
	{
		_addressRegister = value;
	}

	void RegisterSet::incAddressRegister(Word value)
	{
		_addressRegister += value;
	}

	Word RegisterSet::getAddressRegister() const
	{
		return _addressRegister;
	}

	void RegisterSet::setDelayTimer(Byte value)
	{
		_delayTimer = value;
	}

	void RegisterSet::decDelayTimer()
	{
		--_delayTimer;
	}

	Byte RegisterSet::getDelayTimer() const
	{
		return _delayTimer;
	}

	void RegisterSet::setSoundTimer(Byte value)
	{
		_soundTimer = value;
	}

	void RegisterSet::decSoundTimer()
	{
		--_soundTimer;
	}

	Byte RegisterSet::getSoundTimer() const
	{
		return _soundTimer;
	}
}
