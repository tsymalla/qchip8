# qchip8
An experimental emulator for the Chip-8 platform using modern C++ and Qt5.

## Building

Please make sure that Qt 5 (I tested it with Qt 5.14.2) is installed on your system and available in your environment.
Please also install cmake >= 3.10 first. You will also need a C++17-compatible compiler. I used msvc on Windows, gcc64 on Linux and the latest Apple Clang++ on Mac OS.

Building is now as simple as

```
mkdir build
cd build
cmake ../
make
```

## Running games

Just open the executable and select a ROM. The ROM will be started automatically.

## Licensing

The emulator is licensed under the MIT license model. Feel free to use the code in your own projects, but please don't forget to mention me as author. 