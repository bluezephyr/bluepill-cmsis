cmake_minimum_required(VERSION 3.7)

# Project settings
project(bluepill-cmsis C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

add_definitions(-DDEBUG -DSTM32F103xB)

# Linker file is generated from STM32CubeMX
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/stm32f103c8.ld)

# Compiler and linker options
# See https://gcc.gnu.org/onlinedocs/gcc/ARM-Options.html for details
add_compile_options(-mcpu=cortex-m3 -mthumb -mthumb-interwork)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-Og -g)
endif ()

add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-mcpu=cortex-m3 -mthumb -mthumb-interwork)
add_link_options(-T ${LINKER_SCRIPT})

# Use the Newlib-nano C lib instead of Newlib
add_link_options(--specs=nano.specs)

add_executable(${PROJECT_NAME}.elf
    src/main.c
    cmsis-st/Source/Templates/system_stm32f1xx.c
    cmsis-st/Source/Templates/gcc/startup_stm32f102xb.s
    ${LINKER_SCRIPT})

target_include_directories(${PROJECT_NAME}.elf PRIVATE
    include
    cmsis-st/Include
    cmsis_5/CMSIS/Core/Include)

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")
