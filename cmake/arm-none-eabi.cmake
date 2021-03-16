set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR arm)

find_program(ARM_ASM arm-none-eabi-gcc)
find_program(ARM_CC arm-none-eabi-gcc)
find_program(ARM_CXX arm-none-eabi-g++)
find_program(ARM_AR arm-none-eabi-ar)
find_program(ARM_OBJCOPY arm-none-eabi-objcopy)
find_program(ARM_OBJDUMP arm-none-eabi-objdump)
find_program(ARM_SIZE arm-none-eabi-size)

# Cross compilers and tools
set(CMAKE_C_COMPILER ${ARM_CC})
set(CMAKE_CXX_COMPILER ${ARM_CXX})
set(CMAKE_ASM_COMPILER ${ARM_ASM})
set(CMAKE_AR ${ARM_AR})
set(CMAKE_OBJCOPY ${ARM_OBJCOPY})
set(CMAKE_OBJDUMP ${ARM_OBJDUMP})
set(SIZE ${ARM_SIZE})
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Linker file generated from STM32CubeMX
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/stm32f103c8.ld)

# Print configuration
message( STATUS "Toolchain CMakefile directory: ${TOOLCHAIN_DIR}")
message( STATUS "MCU: ${MCU}" )
message( STATUS "CPU Frequency: ${F_CPU} Hz" )
message( STATUS "BAUD Rate: ${BAUD}" )

# Cross-compile version of the add_executable command
function(cc_add_executable NAME)
    if(NOT ARGN)
        message(FATAL_ERROR "No source files for ${NAME}.")
    endif(NOT ARGN)

    set(ELF_FILE ${NAME}.elf)
    set(HEX_FILE ${NAME}.hex)
    set(BIN_FILE ${NAME}.bin)
    set(MAP_FILE ${NAME}.map)

    # elf file
    add_executable(
        ${ELF_FILE} EXCLUDE_FROM_ALL
        ${ARGN}
        cmsis/cmsis-st/Source/system_stm32f1xx.c
        cmsis/cmsis-st/Source/gcc/startup_stm32f103xb.s
        ${LINKER_SCRIPT}
    )
    add_custom_target(${NAME} ALL DEPENDS ${HEX_FILE})

    # Compiler and linker options
    # See https://gcc.gnu.org/onlinedocs/gcc/ARM-Options.html for details
    target_compile_options(
        ${ELF_FILE} PRIVATE
        -mcpu=cortex-m3 -mthumb -mthumb-interwork
        -ffunction-sections -fdata-sections -fno-common -fmessage-length=0
    )

    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
        message(STATUS "Maximum optimization for speed")
        target_compile_options(${ELF_FILE} PRIVATE -Ofast)
    elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
        message(STATUS "Maximum optimization for speed, debug info included")
        target_compile_options(${ELF_FILE} PRIVATE -Ofast -g)
    elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
        message(STATUS "Maximum optimization for size")
        target_compile_options(${ELF_FILE} PRIVATE -Os)
    else ()
        message(STATUS "Minimal optimization, debug info included")
        target_compile_options(${ELF_FILE} PRIVATE -Og -g)
    endif ()

    target_link_options(
        ${ELF_FILE} PRIVATE
        -mcpu=cortex-m3
        -mthumb -mthumb-interwork
        -T ${LINKER_SCRIPT}
        -Wl,-gc-sections,--print-memory-usage,-Map=${MAP_FILE}
    )

    # Use the Newlib-nano C lib instead of Newlib
    target_link_options(${ELF_FILE} PRIVATE --specs=nano.specs)

    # Save the original name to be used in the cc_target_include_directories
    # and cc_target_link_libraries function calls
    set_target_properties( ${NAME} PROPERTIES OUTPUT_NAME "${ELF_FILE}" )

    add_custom_command(
        OUTPUT  ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Oihex ${ELF_FILE} ${HEX_FILE}
        COMMENT "Building ${HEX_FILE}"
        DEPENDS ${ELF_FILE}
        VERBATIM
    )

    add_custom_command(
        OUTPUT  ${BIN_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary ${ELF_FILE} ${BIN_FILE}
        COMMENT "Building ${BIN_FILE}"
        DEPENDS ${ELF_FILE}
        VERBATIM
    )

    # Flash target
    #add_custom_target(
    #        flash
    #        COMMAND ${AVRDUDE} -p ${MCU} -c ${AVR_PROGRAMMER} ${AVR_PROGRAMMER_ARGS} ${AVR_PROGRAMMER_PORT} -U flash:w:${HEX_FILE}
    #        DEPENDS ${HEX_FILE}
    #        COMMENT "Flashing ${HEX_FILE} to ${MCU} using ${AVR_PROGRAMMER}"
    #        VERBATIM
    #)

    # Assembler listing
    add_custom_target(
            list
            COMMAND ${CMAKE_OBJDUMP} -h -S ${ELF_FILE} > ${NAME}.lst
            DEPENDS ${ELF_FILE}
            COMMENT "Creating assembler listing in ${NAME}.lst based on ${ELF_FILE}"
            VERBATIM
    )

    # Size
    add_custom_target(
            size
            COMMAND ${SIZE} ${ELF_FILE}
            DEPENDS ${ELF_FILE}
            COMMENT "Size of the binary ${ELF_FILE}"
            VERBATIM
    )

endfunction(cc_add_executable)


# Cross-compile version of the target_include_directories command
function(cc_target_include_directories NAME)
    get_target_property(TARGET ${NAME} OUTPUT_NAME)
    target_include_directories(${TARGET} ${ARGN})
endfunction(cc_target_include_directories)


# Cross-compile version of the target_link_libraries command
function(cc_target_link_libraries NAME)
    get_target_property(TARGET ${NAME} OUTPUT_NAME)
    target_link_libraries(${TARGET} ${ARGN})
endfunction(cc_target_link_libraries)

