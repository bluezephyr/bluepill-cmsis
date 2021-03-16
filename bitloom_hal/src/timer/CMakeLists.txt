add_library(timer
        timer.c
        )

target_include_directories(timer PRIVATE ${BITLOOM_CORE}/include)
target_include_directories(timer PRIVATE ${BITLOOM_HAL}/../cmsis/cmsis-st/Include)
target_include_directories(timer PRIVATE ${BITLOOM_HAL}/../cmsis/cmsis_5/CMSIS/Core/Include)
