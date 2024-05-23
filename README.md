# RISC-V Emulator Testing Library

This repository contains the system/kernel operations as well as test programs for my custom RISC-V Emulator [RV32IMEmulator](https://github.com/ryanriccio1/RV32IMEmulator). It uses a custom CMake toolchain setup to utilize the [riscv-gnu-toolchain](https://github.com/riscv-collab/riscv-gnu-toolchain).

## Building

Using either a supported Linux distribution or WSL, after installing dependencies, build [riscv-gnu-toolchain](https://github.com/riscv-collab/riscv-gnu-toolchain) using the following commands:

```
./configure --prefix=/opt/riscv --with-arch=rv32im --with-abi=ilp32
make
```

Then, with CMake installed, from the root folder of the repository, initialize the directory to use CMake with the given toolchain file:

```
cmake -DCMAKE_TOOLCHAIN_FILE=/lib/rv32im.cmake .
```

The current `CMakeLists.txt` will build all test programs and place them in the parent folder accordingly. It generates ".elf" (full executable, ready to read with obj-dump) and ".bin" (binary files, used to load memory contents of emulator) files. It can be run using:

```
cmake --build .
```

## Using the "kernel"

The entire kernel implementation can be pick and choose, but it is easiest just to `#include <computer.h>` and use the features from there. Once we do this, it gives us access to the entire API for controlling the hardware provided to us by the computer. It can be broken down into the following sections:

1. [computer.h](include/computer.h)
   - The basic interface for setting up external parts of the emulator such as interrupts and graphics. Allows programmer to select video mode using `setup_character_mode()` and `setup_bitmap_mode()`.
2. [common.h](include/common.h)
   - Contains defaults for given memory locations, as well as constants for the entire program. Offsets and memory addresses are updated during video mode setup and can be accessible by programmer for future use.
3. [font.h](include/font.h)
   - Default array storing 8x8 font information. Font is very similar to ASCII, with a few minor changes for useability.
4. [mem.h](include/mem.h)
   - Simple templated functions for memory operations.
5. [riscv-stdio.h](include/riscv-stdio.h), [riscv-stdio-c.h](include/riscv-stdio-c.h), [riscv-stdio-c.cpp](lib/sys/riscv-stdio-c.cpp)
   - Provides functions for reading and writing to both screen and UART, as well as a `clear()` function and a C wrapper for use in non-CPP applications.
6. [input_buffer.h](include/input_buffer.h)
   - Generic input buffer class for storing user input.
7. [irq.h](include/irq.h), [irq.cpp](lib/sys/irq.cpp)
   - Default setup for interrupts. When this file is included, interrupts are automatically enabled. Interrupt handlers are of type `std::function<void(void)>` and are inlined to be overwritten later by either setup function, or the programmer.
8. [vga.h](include/vga.h)
   - Contains structures and classes for controlling video output as well as cursor movement. See [image_prg.cpp](lib/image_prg/image_prg.cpp) for example usage.
9. [syscalls.cpp](lib/sys/syscalls.cpp)
   - Implementation of system calls used by Newlib, pointed towards our kernel implementation.
10. [boot.s](lib/sys/boot.s)
    - Entry for program. In its current state, should not be modified. Used for initializing stack pointer, global pointer, argc, argv, envp, IRQs, and calling main function.

## Programs

1. [test_prg.elf](lib/test_program/test_program.cpp)
   - Simple test program to test the toolchain, heap allocations using smart pointers, range-based for loops, vector operations, and `std::string` operations, as well as some newer C++ features such as `std::format`.
2. [asm_test.elf](lib/asm_test/main.s)
   - Assembly playground, no real program stored here.
3. [basic.elf](lib/basic/basic.cpp)
   - Miniature basic interpreter built using [ubasic](https://github.com/adamdunkels/ubasic).
4. [image_prg.elf](lib/image_prg/image_prg.cpp)
   - Use character mode to display a precalculated gradient across the screen, covering the screen slowly using a mix of timer interrupts and the `std::mt19937` random device with a uniform integer distribution to cover the screen evenly.
5. [bitmap_prg.elf](lib/image_prg/bitmap_prg.cpp)
   - Display a 24-bit color, RGB image to the screen in bitmap mode to show the capabilities of the graphics controller, then after a key press, display frames from a precalculated animation.
6. [snake.elf](lib/snake/snake.cpp)
   - Simple snake game to show user input capabilities.

## Changing Memory Size

The size of RAM allocated by the emulator is determined by the loaded program, determined at compile time. To adjust the size of memory, the value of `ram_size` must be changed in [common.h](https://github.com/ryanriccio1/RV32IMLib/blob/master/include/common.h#L4) as well as in the [linker script](https://github.com/ryanriccio1/RV32IMLib/blob/master/linker.lds#L1) as the constant `RAM_SIZE`. The size of RAM must be a power of 2. The linker script also has the power to adjust the heap size as well using `__heap_size`.

## Tools

### [generate_bins.sh](generate_bins.sh)

Post-processing script used to generate compatible binary files for the emulator, as well as placing these binaries in a suitable location for testing.

### [gen_image.py](lib/image_gen/gen_image.py)

A very quick and dirty python program for dumping RGB data from an image to a C-style array for image loading.
