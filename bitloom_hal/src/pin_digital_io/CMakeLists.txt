add_library(pin_digital_io
        pin_digital_io.c
        )

target_include_directories(pin_digital_io PRIVATE ${BITLOOM_CORE}/include)
target_include_directories(pin_digital_io PRIVATE ${BITLOOM_HAL}/../cmsis/cmsis-st/Include)
target_include_directories(pin_digital_io PRIVATE ${BITLOOM_HAL}/../cmsis/cmsis_5/CMSIS/Core/Include)
