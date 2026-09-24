/**
 * @file      app_flrp_img_xfer.h
 *
 * @brief     FLRP image transfer application definitions
 *
 * The Clear BSD License
 * Copyright Semtech Corporation 2026. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted (subject to the limitations in the disclaimer
 * below) provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Semtech corporation nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY
 * THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
 * NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SEMTECH CORPORATION BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef APP_FLRP_IMG_XFER_H
#define APP_FLRP_IMG_XFER_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>
#include <stdbool.h>

#include "smtc_flrp_api.h"

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC MACROS -----------------------------------------------------------
 */

/*!
 * @brief Application aliases for @ref smtc_flrp_frequency_plan_t.
 */
#define SMTC_FLRP_FREQ_SUB_G SMTC_FLRP_FREQ_865MHz
#define SMTC_FLRP_FREQ_2G4 SMTC_FLRP_FREQ_2GHz4

#ifndef FLRP_IMG_XFER_CRYPTO_ENABLED
#define FLRP_IMG_XFER_CRYPTO_ENABLED true
#endif

#ifndef FLRP_IMG_XFER_IS_LOW_FREQUENCY
#define FLRP_IMG_XFER_IS_LOW_FREQUENCY true
#endif

#ifndef FLRP_IMG_XFER_INITIATOR_PERIODIC_MS
#define FLRP_IMG_XFER_INITIATOR_PERIODIC_MS 2000
#endif

#ifndef FLRP_IMG_XFER_BUTTON_LONG_PRESS_MS
#define FLRP_IMG_XFER_BUTTON_LONG_PRESS_MS 2000
#endif

#ifndef FLRP_IMG_XFER_BUTTON_DEBOUNCE_MS
#define FLRP_IMG_XFER_BUTTON_DEBOUNCE_MS 50
#endif

#ifndef FLRP_IMG_XFER_CRYSTAL_ERROR
#define FLRP_IMG_XFER_CRYSTAL_ERROR 10
#endif

/*!
 * @brief The number of images to transmit (42 frames).
 */
#define IMAGE_NUMS 42

/*!
 * @brief Single image size: 128x64 monochrome = 1024 bytes.
 */
#define SINGLE_IMAGE_SIZE 1024

/*!
 * @brief Total payload size: 42 KBytes.
 */
#define TOTAL_IMAGES_SIZE ( IMAGE_NUMS * SINGLE_IMAGE_SIZE )

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/*!
 * @brief Slave RX statistics reported to the initiator after each image transfer.
 */
typedef struct
{
    int32_t  rssi_mean;
    uint32_t nb_packets_received_ok;
    uint32_t nb_packets_expected;
} flrp_slave_rx_report_t;

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */

/*!
 * @brief  Initialize the FLRP image transfer application.
 */
bool flrp_img_xfer_init( void );

/*!
 * @brief  Start the FLRP image transfer.
 */
bool flrp_img_xfer_start_transfer( void );

/*!
 * @brief Get the event semaphore used to wake the main thread.
 *
 * @details Given by the button ISR and FLRP TX/RX callbacks; taken by main with a timeout.
 */
struct k_sem* flrp_img_xfer_get_event_sem( void );

#ifdef __cplusplus
}
#endif

#endif  // APP_FLRP_IMG_XFER_H

/* --- EOF ------------------------------------------------------------------ */