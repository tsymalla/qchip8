#ifndef MEMORY_H
#define MEMORY_H

#include "datatypes.h"
#include <QByteArray>

namespace Chip8
{
	class Memory
	{
	  private:
		constexpr static size_t MEMORY_SIZE = 4096;
		constexpr static size_t ROM_START = 0x200;

		StaticByteArray<MEMORY_SIZE> _memory;

		void _loadFontMap();

	  public:
		Memory();
		void resetMemory();

		void writeByte(size_t offset, Byte byte);
		Byte readByte(const size_t offset) const;
		Byte& readByte(const size_t offset);

		Word readWord(const size_t offset) const;

		Byte operator[](const size_t offset) const;
		Byte& operator[](const size_t offset);

		Memory& operator=(const QByteArray& data);
	};
} // namespace Chip8

#endif // MEMORY_H
