# Blue Pill CMSIS

Repository to build firmware for the Blue Pill board. See below for details.
The firmware only uses
[CMSIS](https://arm-software.github.io/CMSIS_5/General/html/index.html).

## Board

See below for some information about the Blue Pill board and its mounted MCU.

* [Blue Pill board](https://stm32-base.org/boards/STM32F103C8T6-Blue-Pill.html)
* MCU: [STM32F103C8](https://www.st.com/en/microcontrollers-microprocessors/stm32f103c8.html).
* External crystal at 8.000 MHz
* User LED at PC13

## CMSIS

The usage of CMSIS in embedded applications based on Cortex-M MCUs is described
in detail [here](https://arm-software.github.io/CMSIS_5/Core/html/using_pg.html).
Basically, two sets of files are needed - one set of files from ARM and one
set of files from the chip vendor. The files are added as submodules to this
repo.

* ARM common CMSIS files can be found [here](https://github.com/ARM-software/CMSIS_5)
* The ST specific CMSIS files can be found [here](https://github.com/topics/cmsis-device)

To init the submodules, use `git submodule update --init`.

### Core CMSIS files

The core parts in order to build and start the software on the target 
hardware are listed below.

*   Linker file: `stm32f103c8.ld`. This file is specific for compiler toolchain
    (gcc). The file is generated from STM32CubeMX. An alternative is to use 
    the file in the ST specific CMSIS files 
    (`cmsis-st/Source/Templates/gcc/linker`) but there seems not to be a 
    file that matches the exact MCU.
    
*   Startup file: `./cmsis-st/Source/Templates/gcc/startup_stm32f102xb.s`.
    This file is part of the ST specific CMSIS files. It is specific for 
    the device (i.e., the specific MCU) as well as the compiler tool chain. The
    code in this file will be executed after a reset (`Reset_Handler`). The 
    code will do things as initialize the RAM memory and call the `SystemInit`
    function (see below). When the initialization is done, the `main` function
    will be called.
    
*   System device file: `cmsis-st/Source/Templates/system_stm32f1xx.c`. This 
    file is used to setup the microcontroller. The function is specified by 
    CMSIS and implemented by the device vendor.
    
*   `main` function. This is the application start that will be called by the
    startup code (asm) when the system has been initialized. The function is 
    located in the `src/main.c` file.
    
## Flash to target

To flash the firmware to the MCU, an
[ST Link](https://www.st.com/en/development-tools/st-link-v2.html) in-circuit
debugger/programmer is used. ST link can also be used to debug using, for 
example the SWD interface. The Open On-Chip Debugger software 
(http://openocd.org/) can be used for this purpose. A board configuration 
file can be found in the `openocd` folder.
