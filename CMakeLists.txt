cmake_minimum_required ( VERSION 3.22 )

project ( RV32IM CXX C ASM)
set ( GEN_BIN ON )

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_C_STANDARD 17)
set ( CMAKE_C_FLAGS "-Wall -ffunction-sections -fdata-sections -nostartfiles -mstrict-align -mbranch-cost=10 -ffixed-x4 -ffixed-tp -Os -Wno-char-subscripts")
set ( CMAKE_CXX_FLAGS "-Wall -ffunction-sections -fdata-sections -fno-use-cxa-atexit -nostartfiles -mstrict-align -mbranch-cost=10 -ffixed-x4 -ffixed-tp -Os -Wno-char-subscripts")#-Wl,--wrap,malloc

set ( LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/linker.lds" )
set ( LINKER_FLAGS "LINKER:-T${LINKER_SCRIPT},-melf32lriscv,--entry=_start,--no-relax,--no-warn-rwx-segments,--static,--gc-sections" ) # ,-lm,-lc,-lgcc,-Map=rv32im.map,--verbose,-lgcc,--start-group,-lc,-lnosys,--end-group,-lgcc
add_link_options(${LINKER_FLAGS})

# ${CMAKE_SOURCE_DIR}/lib/mem/new.cpp
set ( BOOT_SCRIPT ${CMAKE_SOURCE_DIR}/lib/sys/boot.s )
set ( SRCS ${CMAKE_SOURCE_DIR}/lib/sys/syscalls.cpp ${CMAKE_SOURCE_DIR}/lib/sys/irq.cpp ${CMAKE_SOURCE_DIR}/lib/sys/riscv-stdio-c.cpp ${BOOT_SCRIPT})
include_directories(${CMAKE_SOURCE_DIR}/include/)
include_directories(${CMAKE_SOURCE_DIR}/lib/basic/)
include_directories(${CMAKE_SOURCE_DIR}/lib/basic/ubasic/)
include_directories(${CMAKE_SOURCE_DIR}/lib/image_prg/)
include_directories(${CMAKE_SOURCE_DIR}/lib/snake/)


add_executable(test_prg.elf ${CMAKE_SOURCE_DIR}/lib/test_program/test_program.cpp ${SRCS}) 
add_executable(asm_test.elf ${CMAKE_SOURCE_DIR}/lib/asm_test/main.s ${SRCS}) 
add_executable(basic.elf ${CMAKE_SOURCE_DIR}/lib/basic/basic.cpp ${CMAKE_SOURCE_DIR}/lib/basic/ubasic/ubasic.c ${CMAKE_SOURCE_DIR}/lib/basic/ubasic/tokenizer.c ${SRCS})
add_executable(image_prg.elf ${CMAKE_SOURCE_DIR}/lib/image_prg/image_prg.cpp ${SRCS})
add_executable(bitmap_prg.elf ${CMAKE_SOURCE_DIR}/lib/image_prg/bitmap_prg.cpp ${SRCS})
add_executable(snake.elf ${CMAKE_SOURCE_DIR}/lib/snake/snake.cpp ${SRCS})

#target_compile_definitions(test_prg.elf PUBLIC EN_IRQ=1)   # enable interrupts for test_prg.elf

if (GEN_BIN)
    add_custom_command(
            COMMAND ./generate_bins.sh
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            OUTPUT test_prg.bin asm_test.bin basic.bin image_prg.bin bitmap_prg.bin snake.bin
            COMMENT "Invoking: Generating Bins"
        )

    add_custom_target(
            genBins ALL
            DEPENDS
            test_prg.bin asm_test.bin basic.bin image_prg.bin bitmap_prg.bin snake.bin
        )  

    add_dependencies(genBins test_prg.elf asm_test.elf basic.elf image_prg.elf bitmap_prg.elf snake.elf)
endif (GEN_BIN)
