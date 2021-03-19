add_library(i2c
        i2c.c
        )

target_include_directories(i2c PRIVATE ${BITLOOM_CORE}/include)
target_include_directories(i2c PRIVATE ${BITLOOM_HAL}/../cmsis/cmsis-st/Include)
target_include_directories(i2c PRIVATE ${BITLOOM_HAL}/../cmsis/cmsis_5/CMSIS/Core/Include)
