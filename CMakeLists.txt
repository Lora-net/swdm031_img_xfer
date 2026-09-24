# Copyright (c) 2026 Semtech Corporation
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(swdm031_img_xfer)

if(CONFIG_USP_FLRP)
    target_compile_definitions(flrp_compile_definitions INTERFACE
        DATA_FLRC_CR=RAL_FLRC_CR_3_4
        DATA_FLRC_MIN_INTERFRAME_DURATION_US=200
        WOR_865MHz_RF_FREQ_IN_HZ=915120000
        TX_OUTPUT_POWER_DBM=22
        WOR_LORA_LONG_PREAMBLE_MS=100
        RESTART_WOR_RX_DELAY_MS=80
    )
    target_link_libraries(app PRIVATE flrp_compile_definitions)
endif()

target_include_directories(app PRIVATE
    src/common
    src/oled_display
)

file(GLOB COMMON src/common/*.c)
file(GLOB OLED_DISPLAY src/oled_display/*.c)

set(IMAGE_DATA "")
if(CONFIG_INITIATOR)
    list(APPEND IMAGE_DATA src/image_data/image_chick_looking.c)
endif()

target_sources(app PRIVATE
    ${COMMON}
    ${OLED_DISPLAY}
    ${IMAGE_DATA}
    src/main.c
)
