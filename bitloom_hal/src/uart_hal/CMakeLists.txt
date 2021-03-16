add_library(uart_hal
        uart_hal.c
        )

target_include_directories(uart_hal PRIVATE ${BITLOOM_CORE}/include)
target_include_directories(uart_hal PRIVATE ${CUTIL}/include)
