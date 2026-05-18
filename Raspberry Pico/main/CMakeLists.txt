# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion RISCV_ZCB_RPI_2_2_0_3)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2 CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(main C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Referecing the library
add_subdirectory(lib/fatfs_sd build)

# Add executable. Default name is the project name, version 0.1
# I am also adding the exec for all c files that I am going to run inside of the project.
add_executable(main main.c images.c
                hw_config.c
                display_config.c
                sd_commands.c)

pico_set_program_name(main "main")
pico_set_program_version(main "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(main 0)
pico_enable_stdio_usb(main 1)

# We are using hardware_spi, and the whole thing library from the no-OS-FatFS-SD-SDIO-SPI-RPi-Pico repo, so we need to link those
# Oh an ofc there is also the pico_stdlib which is required for all Pico projects
target_link_libraries(main
        pico_stdlib
        hardware_spi
        no-OS-FatFS-SD-SDIO-SPI-RPi-Pico
        )

# All of this is inside a directory called main so ofc it must be included with everything else inside of it
target_include_directories(main PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)



pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 0)

pico_add_extra_outputs(main)