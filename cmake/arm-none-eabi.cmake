set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(GCC_ROOT /opt/gcc-arm/bin)

# Cross compilers and tools
set(CMAKE_C_COMPILER ${GCC_ROOT}/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER ${GCC_ROOT}/arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER ${GCC_ROOT}/arm-none-eabi-gcc)
set(CMAKE_AR ${GCC_ROOT}/arm-none-eabi-ar)
set(CMAKE_OBJCOPY ${GCC_ROOT}/arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP ${GCC_ROOT}/arm-none-eabi-objdump)
set(SIZE ${GCC_ROOT}/arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
