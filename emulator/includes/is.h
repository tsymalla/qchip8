#ifndef IS_H
#define IS_H

#include "datatypes.h"
#include "memory.h"
#include "registerset.h"
#include <QObject>

namespace Chip8
{
	class IS : QObject
	{
	  public:
		IS(Word& programCounter, RegisterSet& registerSet, Memory& memory, FrameBuffer& framebuffer,
		   KeyBuffer& keybuffer, QObject* parent = nullptr);

		bool step(const Word& opcode);

	  private:
		Word& _programCounter;
		RegisterSet& _registerSet;
		Memory& _memory;
		FrameBuffer& _framebuffer;
		KeyBuffer& _keybuffer;
		void _stepProgramCounterByte();
	};
} // namespace Chip8

#endif // IS_H
