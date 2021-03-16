# Blue Pill CMSIS

Repository to build firmware for the Blue Pill board. See below for details.
The firmware only uses
[CMSIS](https://arm-software.github.io/CMSIS_5/General/html/index.html).

## Board

See below for some information about the Blue Pill board and its mounted MCU.

* [Blue Pill board](https://stm32-base.org/boards/STM32F103C8T6-Blue-Pill.html)
* [STM32F103C8](https://www.st.com/en/microcontrollers-microprocessors/stm32f103c8.html)
* External crystal at 8.000 MHz
* User LED at PC13

## CMSIS

The usage of CMSIS in embedded applications based on Cortex-M MCUs is described
in detail [here](https://arm-software.github.io/CMSIS_5/Core/html/using_pg.html).
Basically, two sets of files are needed - one set of files from ARM and one
set of files from the chip vendor. All files can be found in the repos below.

* ARM common CMSIS files can be found [here](https://github.com/ARM-software/CMSIS_5)
* The ST specific CMSIS files can be found [here](https://github.com/topics/cmsis-device)

The relevant CMSIS files for the MCU have been copied to the `cmsis` folder for
easy access.

### Core CMSIS files

The core parts in order to build and start the software on the target 
hardware are listed below.

*   Linker file: `stm32f103c8.ld`. This file is specific for compiler toolchain
    (gcc). The file is generated from STM32CubeMX. An alternative is to use
    the file in the ST specific CMSIS files
    (`cmsis-st/Source/Templates/gcc/linker`) but there seems not to be a 
    file that matches the exact MCU.

*   Startup file: `./cmsis/cmsis-st/Source/gcc/startup_stm32f103xb.s`.
    This file is part of the ST specific CMSIS files. It is specific for
    the device (i.e., the specific MCU) as well as the compiler tool chain. The
    code in this file will be executed after a reset (`Reset_Handler`). The
    code will do things as initialize the RAM memory and call the `SystemInit`
    function (see below). When the initialization is done, the `main` function
    will be called.

*   System device file: `cmsis/cmsis-st/Source/system_stm32f1xx.c`. This
    file is used to setup the microcontroller. The function is specified by
    CMSIS and implemented by the device vendor.

*   `main` function. This is the application start that will be called by the
    startup code (asm) when the system has been initialized. The function is
    located in the `src/main.c` file.

## Build

The software is built using a cross compiling toolchain for ARM and the build
is conducted using [CMake](https://cmake.org/). The cross compilation toolchain
for ARM can found
[here](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm).

To use the ARM toolchain from CMake, use the `cmake/arm-none-eabi.cmake`
toolchain file (`CMAKE_TOOLCHAIN_FILE`). The toolchain file will search for
the needed programs (using `find_program`). Make sure that the PATH environment
variable includes the needed paths or specify the directory with
`CMAKE_PREFIX_PATH`.

### Example - build on Linux

Follow the steps below to build the on Linux. Assuming that the cross
compilation toolchain is installed in `/opt/gcc-arm`.

    mkdir build
    cd build
    cmake -DCMAKE_PREFIX_PATH=/opt/gcc-arm -DCMAKE_TOOLCHAIN_FILE=../cmake/arm-none-eabi.cmake ..
    make

## Flash to target

To flash the firmware to the MCU, an
[ST Link](https://www.st.com/en/development-tools/st-link-v2.html) in-circuit
debugger/programmer is used. ST link can also be used to debug using, for
example the SWD interface. The Open On-Chip Debugger software
(http://openocd.org/) can be used for this purpose. A board configuration
file can be found in the `openocd` folder.

## Configuration

### Memory

No heap memory is used. The linker file is updated to reflect this.

### GPIO

The User LED mounted on the board and connected to PC13 is enabled as General
Purpose output push-pull. Note that the LED is active low.

### System clocks

From the data sheet [RM0008] section 7:

_Several prescalers allow the configuration of the AHB frequency, the high
speed APB (APB2) and the low speed APB (APB1) domains. The maximum frequency
of  the AHB and the APB2 domains is 72 MHz. The maximum allowed frequency of the
APB1 domain is 36 MHz. The SDIO AHB interface is clocked with a fixed frequency
equal to HCLK/2_

To use the external crystal (MHz) the configuration below is used. This will
result in a 72MHz system clock: HSE 8MHz crystal with PLL x 9 (8MHz x 9 = 72
MHz).

From the data sheet [RM0008] section 3.3.3:

_Reading the Flash memory
Flash memory instructions and data access are performed through the AHB bus.
... Flash access control register (FLASH_ACR) ... LATENCY Two wait states, if
48 MHz < SYSCLK <= 72 MHz_

Step 1
* Configure FLASH_ACR = Flash latency two wait states
* Enable HSE (High Speed External clock signal) from crystal. Wait for the
  HSERDY flag to be set indicating that the clock has stabilized.

Step 2
* AHB Prescaler = /1 -- Maximum allowed frequency is 36MHz
* PLLMul = x9
* Set PLL Source Mux to HSE
* Configure PLL and then enable. Wait for the PLL to be ready (PLLRDY)

Step 3
* Set System Clock Mux source to PLL


## References

[RM0008](https://www.st.com/resource/en/reference_manual/cd00171190-stm32f101xx-stm32f102xx-stm32f103xx-stm32f105xx-and-stm32f107xx-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf)
: Reference manual STM32F101xx, STM32F102xx, STM32F103xx, STM32F105xx and
STM32F107xx advanced Arm-based 32-bit MCUs
