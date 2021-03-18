add_library(i2c
        i2c.c
        )

target_include_directories(i2c PRIVATE ${BITLOOM_CORE}/include)
