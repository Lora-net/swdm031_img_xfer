/**
 * @file      app_flrp_img_xfer.c
 *
 * @brief     FLRP image transfer application
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

#include <string.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <smtc_modem_hal.h>

#include "smtc_flrp_api.h"
#include "smtc_flrp_api_tests.h"
#include "smtc_flrp_crypto.h"
#include "app_flrp_img_xfer.h"
#include "oled_display.h"

LOG_MODULE_REGISTER( flrp_img_xfer, LOG_LEVEL_INF );

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

static const uint8_t flrp_key[16] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                      0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };

#if CONFIG_INITIATOR
static const uint8_t device_eui[SMTC_FLRP_EUI_LENGTH]        = { 0xFF, 0xFE, 0xFD, 0xFC, 0xFB, 0xFA, 0xF9, 0xF8 };
static const uint8_t target_device_eui[SMTC_FLRP_EUI_LENGTH] = { 0x00, 0xFE, 0xFD, 0xFC, 0xFB, 0xFA, 0xF9, 0xF8 };
extern const uint8_t image_chick_looking_buf[IMAGE_NUMS][SINGLE_IMAGE_SIZE];
#else
static const uint8_t device_eui[SMTC_FLRP_EUI_LENGTH] = { 0x00, 0xFE, 0xFD, 0xFC, 0xFB, 0xFA, 0xF9, 0xF8 };
#endif /* CONFIG_INITIATOR */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

/* Initial count 0, max 1: signals main that a button press or FLRP event occurred. */
K_SEM_DEFINE( flrp_img_xfer_event_sem, 0, 1 );

#if CONFIG_INITIATOR
static uint32_t               flrp_img_xfer_tx_count              = 0;
static uint32_t               flrp_img_xfer_tx_start_timestamp_ms = 0;
static uint32_t               flrp_img_xfer_transaction_ms        = 0;
static flrp_slave_rx_report_t slave_report_rx_buf                 = { 0 };
static bool                   awaiting_slave_report               = false;
static bool                   transfer_session_active             = false;

/* Set when a TX session ends; main.c resets the periodic idle timer (not in the public API). */
volatile bool flrp_img_xfer_session_idle_tick = false;

static void mark_transfer_session_idle( void )
{
    transfer_session_active         = false;
    flrp_img_xfer_session_idle_tick = true;
}
#else
static uint32_t               flrp_img_xfer_rx_count = 0;
static uint8_t                rx_image_buf[TOTAL_IMAGES_SIZE];
static uint8_t                display_image_snapshot[TOTAL_IMAGES_SIZE];  // Cache, to prevent overwrite
static flrp_slave_rx_report_t slave_rx_report = { 0 };
#endif /* CONFIG_INITIATOR */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

static void wake_main_thread( void );

#if CONFIG_INITIATOR
static smtc_flrp_com_config_t flrp_img_xfer_get_com_config( void );
static bool                   flrp_img_xfer_fetch_slave_report( void );
#else
static void slave_prepare_rx_report( const smtc_flrp_burst_rx_stats_t* burst );
static void slave_refresh_display( const smtc_flrp_rx_stats_t* stats, uint32_t payload_size );
#endif /* CONFIG_INITIATOR */

static void flrp_img_xfer_tx_callback( const void* context, smtc_flrp_return_code_t err_code, bool send_successful,
                                       uint8_t* dest_addr );
static void flrp_img_xfer_rx_callback( const void* context, smtc_flrp_return_code_t status, uint32_t payload_size,
                                       uint8_t* src_addr, smtc_flrp_rx_stats_t payload_stats );

static void        print_radio_config( smtc_flrp_radio_config_t cfg, smtc_flrp_flrc_advanced_radio_config_t adv_cfg );
static void        display_radio_config( smtc_flrp_radio_config_t cfg, smtc_flrp_flrc_advanced_radio_config_t adv_cfg );
static uint16_t    radio_flrc_raw_bit_rate_to_value( ral_flrc_raw_bit_rate_t value );
static const char* radio_flrc_cr_to_str( ral_flrc_cr_t value );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

bool flrp_img_xfer_init( void )
{
    smtc_flrp_api_config_t  flrp_config;
    smtc_flrp_return_code_t return_code = SMTC_FLRP_RC_OK;

    flrp_config.crypto_enabled = FLRP_IMG_XFER_CRYPTO_ENABLED;
    flrp_config.freq_plan      = FLRP_IMG_XFER_IS_LOW_FREQUENCY ? SMTC_FLRP_FREQ_SUB_G : SMTC_FLRP_FREQ_2G4;
    flrp_config.crystal_error  = FLRP_IMG_XFER_CRYSTAL_ERROR;
    memcpy( flrp_config.dev_eui, device_eui, SMTC_FLRP_EUI_LENGTH );

    return_code = smtc_flrp_init( flrp_config, flrp_img_xfer_tx_callback, flrp_img_xfer_rx_callback, NULL );
    if( return_code != SMTC_FLRP_RC_OK )
    {
        LOG_ERR( "smtc_flrp_init failed (%u)", return_code );
        return false;
    }

    smtc_flrp_crypto_init( );
    if( smtc_flrp_crypto_set_key( SMTC_SE_APP_KEY, ( uint8_t* ) flrp_key, 0 ) != SMTC_FLRP_CRYPTO_RC_SUCCESS )
    {
        LOG_ERR( "smtc_flrp_crypto_set_key failed" );
        return false;
    }

    smtc_flrp_radio_config_t               cfg     = smtc_flrp_get_current_radio_config( );
    smtc_flrp_flrc_advanced_radio_config_t adv_cfg = smtc_flrp_get_current_advanced_flrc_radio_config( );

    /*
     * Optional runtime overrides for TX power, burst target PER, FLRC raw bit rate, and coding rate.
     * Defaults come from flrp_configuration.h; for build-time changes, use EXTRA_CFLAGS (see README).
     * The initiator and slave must use the same radio parameters. Uncomment the block below to apply.
     */
    // /* LR20XX: Range [-9, +22] for sub-G, range [-19, 12] for 2.4G ( HF_PA ) */
    // cfg.flrc.tx_power_in_dbm  = 22;
    // cfg.flrc.burst_target_per = 100;  // No retry. No retransmission.
    // /* RAL_FLRC_RAW_BIT_RATE_2_600_MBPS RAL_FLRC_RAW_BIT_RATE_2_080_MBPS RAL_FLRC_RAW_BIT_RATE_1_300_MBPS
    //  * RAL_FLRC_RAW_BIT_RATE_1_040_MBPS RAL_FLRC_RAW_BIT_RATE_0_650_MBPS RAL_FLRC_RAW_BIT_RATE_0_520_MBPS
    //  * RAL_FLRC_RAW_BIT_RATE_0_325_MBPS RAL_FLRC_RAW_BIT_RATE_0_260_MBPS */
    // cfg.flrc.raw_bit_rate = RAL_FLRC_RAW_BIT_RATE_2_600_MBPS;
    // return_code           = smtc_flrp_set_new_radio_config( cfg );
    // if( return_code != SMTC_FLRP_RC_OK )
    // {
    //     LOG_ERR( "smtc_flrp_set_new_radio_config failed (%u)", return_code );
    //     return false;
    // }

    // adv_cfg.cr  = RAL_FLRC_CR_1_2;  // RAL_FLRC_CR_3_4 RAL_FLRC_CR_1_2
    // return_code = smtc_flrp_set_new_advanced_flrc_radio_config( adv_cfg );
    // if( return_code != SMTC_FLRP_RC_OK )
    // {
    //     LOG_ERR( "smtc_flrp_set_new_advanced_flrc_radio_config failed (%u)", return_code );
    //     return false;
    // }

    print_radio_config( cfg, adv_cfg );
    display_radio_config( cfg, adv_cfg );

#if CONFIG_INITIATOR
    LOG_INF( "Initiator init OK. (image data size = %u bytes, %u frames)", TOTAL_IMAGES_SIZE, IMAGE_NUMS );
#else
    display_text( DISPLAY_ROW4, "Listening..." );
    memset( rx_image_buf, 0xFF, sizeof( rx_image_buf ) );

    return_code = smtc_flrp_start_periodic_listening( rx_image_buf, sizeof( rx_image_buf ) );
    if( return_code != SMTC_FLRP_RC_OK )
    {
        LOG_ERR( "smtc_flrp_start_periodic_listening failed (%u)", return_code );
        return false;
    }
    slave_prepare_rx_report( NULL );
    LOG_INF( "Slave init OK. (image data size = %u bytes, %u frames)", TOTAL_IMAGES_SIZE, IMAGE_NUMS );
#endif /* CONFIG_INITIATOR */

    return true;
}

struct k_sem* flrp_img_xfer_get_event_sem( void )
{
    return &flrp_img_xfer_event_sem;
}

bool flrp_img_xfer_start_transfer( void )
{
#if CONFIG_INITIATOR
    if( transfer_session_active )
    {
        return false;
    }

    smtc_flrp_com_config_t com_config = flrp_img_xfer_get_com_config( );

    transfer_session_active = true;
    flrp_img_xfer_tx_count++;
    display_text( DISPLAY_ROW4, "TX image( #%u )...", flrp_img_xfer_tx_count );
    printk( "\r\n" );
    LOG_INF( "TX image ( #%u )...", flrp_img_xfer_tx_count );

    flrp_img_xfer_tx_start_timestamp_ms = smtc_modem_hal_get_time_in_ms( );
    smtc_flrp_return_code_t return_code =
        smtc_flrp_initiate_transmission( ( uint8_t* ) image_chick_looking_buf, TOTAL_IMAGES_SIZE, com_config );
    if( return_code != SMTC_FLRP_RC_OK )
    {
        mark_transfer_session_idle( );
        LOG_ERR( "smtc_flrp_initiate_transmission failed (%u)", return_code );
        display_text( DISPLAY_ROW5, "TX start fail" );
        return false;
    }
    LOG_INF( "TX started (%u bytes, %u frames)", TOTAL_IMAGES_SIZE, IMAGE_NUMS );
    return true;
#else
    return false;
#endif
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

static void wake_main_thread( void )
{
    k_sem_give( &flrp_img_xfer_event_sem );
}

#if CONFIG_INITIATOR
static smtc_flrp_com_config_t flrp_img_xfer_get_com_config( void )
{
    smtc_flrp_com_config_t com_config = {
        .com_mode             = SMTC_FLRP_BIDIRECTIONAL,
        .link_adaptation_mode = SMTC_FLRP_LINK_ADAPTATION_DISABLED,
    };

    memcpy( com_config.slave_dev_eui, target_device_eui, SMTC_FLRP_EUI_LENGTH );
    return com_config;
}

static bool flrp_img_xfer_fetch_slave_report( void )
{
    smtc_flrp_return_code_t return_code = smtc_flrp_initiate_reception(
        ( uint8_t* ) &slave_report_rx_buf, sizeof( slave_report_rx_buf ), flrp_img_xfer_get_com_config( ) );
    if( return_code != SMTC_FLRP_RC_OK )
    {
        LOG_ERR( "smtc_flrp_initiate_reception failed (%u)", return_code );
        return false;
    }

    LOG_INF( "Fetching slave RX report..." );
    display_text( DISPLAY_ROW5, "Fetch stats..." );
    return true;
}

#else

static void slave_prepare_rx_report( const smtc_flrp_burst_rx_stats_t* burst )
{
    if( burst != NULL )
    {
        slave_rx_report.rssi_mean              = burst->rssi_mean;
        slave_rx_report.nb_packets_received_ok = burst->nb_packets_received_ok;
        slave_rx_report.nb_packets_expected    = burst->nb_packets_expected;
    }
    else
    {
        memset( &slave_rx_report, 0, sizeof( slave_rx_report ) );
    }

    smtc_flrp_return_code_t return_code =
        smtc_flrp_slave_prepare_data_to_send( ( uint8_t* ) &slave_rx_report, sizeof( slave_rx_report ) );
    if( return_code != SMTC_FLRP_RC_OK )
    {
        LOG_ERR( "smtc_flrp_slave_prepare_data_to_send failed (%u)", return_code );
    }
}

static void slave_refresh_display( const smtc_flrp_rx_stats_t* stats, uint32_t payload_size )
{
    const smtc_flrp_burst_rx_stats_t*      burst   = &stats->burst;
    const smtc_flrp_wor_rx_stats_t*        wor     = &stats->wor;
    smtc_flrp_radio_config_t               cfg     = smtc_flrp_get_current_radio_config( );
    smtc_flrp_flrc_advanced_radio_config_t adv_cfg = smtc_flrp_get_current_advanced_flrc_radio_config( );
    uint32_t                               bitfield_len;
    uint32_t                               per_x10;

    display_radio_config( cfg, adv_cfg );

    LOG_INF( "Size=%u ok/err/nok=%u/%u/%u expected=%u rssi=%d dBm wor_rssi=%d dBm wor_snr=%d dB", payload_size,
             burst->nb_packets_received_ok, burst->nb_packets_check_error, burst->nb_packets_received_nok,
             burst->nb_packets_expected, burst->rssi_mean, wor->rssi, wor->snr );

    per_x10 =
        burst->nb_packets_expected > 0U
            ? ( ( burst->nb_packets_expected - burst->nb_packets_received_ok ) * 1000U ) / burst->nb_packets_expected
            : 100U;
    LOG_INF( "PER: %u.%u %% (OK %u / All %u packets)", per_x10 / 10U, per_x10 % 10U, burst->nb_packets_received_ok,
             burst->nb_packets_expected );

    bitfield_len = CEIL_DIVISION( burst->nb_packets_expected, 8 );
    if( bitfield_len > SMTC_FLRP_BITFIELD_PACKETS_IN_BURST_LENGTH )
    {
        bitfield_len = SMTC_FLRP_BITFIELD_PACKETS_IN_BURST_LENGTH;
    }
    LOG_HEXDUMP_INF( burst->last_burst_missed_packets_bitfield, bitfield_len, "Missed packets bitfield (1=missed)" );

    display_text( DISPLAY_ROW4, "OK ( %u ) / All ( %u )", burst->nb_packets_received_ok, burst->nb_packets_expected );
    display_text( DISPLAY_ROW5, "RSSI = %d dBm", burst->rssi_mean );
    display_text( DISPLAY_ROW6, "PER: %u.%u %%", per_x10 / 10U, per_x10 % 10U );
}

#endif /* CONFIG_INITIATOR */

static void flrp_img_xfer_tx_callback( const void* context, smtc_flrp_return_code_t err_code, bool send_successful,
                                       uint8_t* dest_addr )
{
    ARG_UNUSED( context );
    ARG_UNUSED( dest_addr );

#if CONFIG_INITIATOR
    if( ( err_code == SMTC_FLRP_RC_OK ) && send_successful )
    {
        flrp_img_xfer_transaction_ms = smtc_modem_hal_get_time_in_ms( ) - flrp_img_xfer_tx_start_timestamp_ms;
        LOG_INF( "TX callback (OK #%u), transaction time %u ms", flrp_img_xfer_tx_count, flrp_img_xfer_transaction_ms );
        awaiting_slave_report = true;
        if( !flrp_img_xfer_fetch_slave_report( ) )
        {
            awaiting_slave_report = false;
            mark_transfer_session_idle( );
            display_text( DISPLAY_ROW4, "Press btn / auto %u s", FLRP_IMG_XFER_INITIATOR_PERIODIC_MS / 1000U );
            display_text( DISPLAY_ROW5, "Stats req fail" );
        }
    }
    else
    {
        mark_transfer_session_idle( );
        LOG_ERR( "TX failed #%u (err=%u, ok=%u)", flrp_img_xfer_tx_count, err_code, send_successful );
        display_text( DISPLAY_ROW5, "TX failed: #%u", flrp_img_xfer_tx_count );
        display_text( DISPLAY_ROW4, "Press btn / auto %u s", FLRP_IMG_XFER_INITIATOR_PERIODIC_MS / 1000U );
    }
#else  /* CONFIG_INITIATOR */
    LOG_INF( "TX callback (err=%u, ok=%u)", err_code, send_successful );
#endif /* CONFIG_INITIATOR */

    wake_main_thread( );
}

static void flrp_img_xfer_rx_callback( const void* context, smtc_flrp_return_code_t status, uint32_t payload_size,
                                       uint8_t* src_addr, smtc_flrp_rx_stats_t payload_stats )
{
    ARG_UNUSED( context );
    ARG_UNUSED( src_addr );

#if CONFIG_INITIATOR
    if( awaiting_slave_report )
    {
        awaiting_slave_report = false;
        mark_transfer_session_idle( );

        if( ( status == SMTC_FLRP_RC_OK ) && ( payload_size >= sizeof( flrp_slave_rx_report_t ) ) )
        {
            uint32_t per_x10 =
                slave_report_rx_buf.nb_packets_expected > 0U
                    ? ( ( slave_report_rx_buf.nb_packets_expected - slave_report_rx_buf.nb_packets_received_ok ) *
                        1000U ) /
                          slave_report_rx_buf.nb_packets_expected
                    : 100U;

            LOG_INF( "Slave report: rssi=%d dBm ok=%u expected=%u PER=%u.%u %%", slave_report_rx_buf.rssi_mean,
                     slave_report_rx_buf.nb_packets_received_ok, slave_report_rx_buf.nb_packets_expected, per_x10 / 10U,
                     per_x10 % 10U );
            display_text( DISPLAY_ROW4, "OK ( %u ) / All ( %u )", slave_report_rx_buf.nb_packets_received_ok,
                          slave_report_rx_buf.nb_packets_expected );
            display_text( DISPLAY_ROW5, "RSSI = %d dBm", slave_report_rx_buf.rssi_mean );
            display_text( DISPLAY_ROW6, "PER: %u.%u %%   All ET: %ums", per_x10 / 10U, per_x10 % 10U,
                          flrp_img_xfer_transaction_ms );
        }
        else
        {
            LOG_ERR( "Slave report failed (status=%u size=%u)", status, payload_size );
            display_text( DISPLAY_ROW4, "Press btn / auto %u s", FLRP_IMG_XFER_INITIATOR_PERIODIC_MS / 1000U );
            display_text( DISPLAY_ROW5, "Stats RX fail" );
        }
    }
    else
    {
        ARG_UNUSED( status );
        ARG_UNUSED( payload_size );
        ARG_UNUSED( payload_stats );
    }

#else
    flrp_img_xfer_rx_count++;
    if( ( status == SMTC_FLRP_RC_OK ) && ( payload_size > 0 ) )
    {
        /* Missing packet regions remain 0x00 (black) in rx_image_buf. */
        memcpy( display_image_snapshot, rx_image_buf, TOTAL_IMAGES_SIZE );
        display_image( display_image_snapshot, TOTAL_IMAGES_SIZE );

        printk( "\r\n" );
        LOG_INF( "[RX #%u OK]", flrp_img_xfer_rx_count );
        slave_refresh_display( &payload_stats, payload_size );
        slave_prepare_rx_report( &payload_stats.burst );
    }
    else
    {
        LOG_ERR(
            "[RX #%u KO] status=%u size=%u ok/err/nok=%u/%u/%u expected=%u rssi=%d dBm "
            "wor_rssi=%d dBm wor_snr=%d dB",
            flrp_img_xfer_rx_count, status, payload_size, payload_stats.burst.nb_packets_received_ok,
            payload_stats.burst.nb_packets_check_error, payload_stats.burst.nb_packets_received_nok,
            payload_stats.burst.nb_packets_expected, payload_stats.burst.rssi_mean, payload_stats.wor.rssi,
            payload_stats.wor.snr );
        display_text( DISPLAY_ROW4, "RX failed: #%u", flrp_img_xfer_rx_count );
    }
    memset( rx_image_buf, 0xFF, sizeof( rx_image_buf ) );
#endif /* CONFIG_INITIATOR */

    wake_main_thread( );
}

static void print_radio_config( smtc_flrp_radio_config_t cfg, smtc_flrp_flrc_advanced_radio_config_t adv_cfg )
{
    printk( "FLRC parameters:\r\n" );
    printk( "    Raw bit rate:    %s\r\n", ral_flrc_raw_bit_rate_to_str( cfg.flrc.raw_bit_rate ) );
    printk( "    Coding rate:     %s\r\n", ral_flrc_cr_to_str( adv_cfg.cr ) );
    printk( "    Pulse shape:     %s\r\n", ral_flrc_pulse_shape_to_str( adv_cfg.pulse_shape ) );
    printk( "    Preamble:        %s\r\n", ral_flrc_preamble_length_to_str( adv_cfg.preambule_len ) );
    printk( "    CRC:             %s\r\n", ral_flrc_crc_type_to_str( adv_cfg.crc_type ) );
    printk( "    Channel:         %u Hz\r\n", cfg.flrc.channels_freq_hz[cfg.flrc.default_channel] );
    printk( "    Tx power:        %d dBm\r\n", cfg.flrc.tx_power_in_dbm );
    printk( "    Target PER:      %u%%\r\n", cfg.flrc.burst_target_per );

    printk( "WOR parameters:\r\n" );
    printk( "    RX frequency:    %u Hz\r\n", cfg.wor_rx.frequency_hz );
    printk( "    TX frequency:    %u Hz\r\n", cfg.wor_tx.frequency_hz );
    printk( "    SF:              %s\r\n", ral_lora_sf_to_str( cfg.wor_tx.sf ) );
    printk( "    BW:              %s\r\n", ral_lora_bw_to_str( cfg.wor_tx.bw ) );
}

static void display_radio_config( smtc_flrp_radio_config_t cfg, smtc_flrp_flrc_advanced_radio_config_t adv_cfg )
{
    display_text( DISPLAY_ROW1, "FLRP %s", IS_ENABLED( CONFIG_SLAVE ) ? "Slave" : "Initiator" );
    uint32_t flrc_default_channel_hz = cfg.flrc.channels_freq_hz[cfg.flrc.default_channel];
    uint32_t integer_part            = flrc_default_channel_hz / 1000000;
    uint32_t decimal_part            = ( flrc_default_channel_hz % 1000000 ) / 10000;
    display_text( DISPLAY_ROW2, "Freq: %u.%uM  Pwr: %d dBm", integer_part, decimal_part, cfg.flrc.tx_power_in_dbm );
    uint16_t phy_kbps      = radio_flrc_raw_bit_rate_to_value( cfg.flrc.raw_bit_rate );
    uint32_t phy_mbps_int  = phy_kbps / 1000U;
    uint32_t phy_mbps_frac = ( phy_kbps % 1000U ) / 10U;
    display_text( DISPLAY_ROW3, "PHY: %u.%02uMbps   %s", phy_mbps_int, phy_mbps_frac,
                  radio_flrc_cr_to_str( adv_cfg.cr ) );
}

static uint16_t radio_flrc_raw_bit_rate_to_value( ral_flrc_raw_bit_rate_t value )
{
    static const uint16_t rates_kbps[] = { 260, 325, 520, 650, 1040, 1300, 2080, 2600 };

    return ( value <= RAL_FLRC_RAW_BIT_RATE_2_600_MBPS ) ? rates_kbps[value] : 0;
}

static const char* radio_flrc_cr_to_str( ral_flrc_cr_t value )
{
    static const char* const labels[] = { "CR: 1/2", "CR: 3/4", "CR: 1/1", "CR: 2/3" };

    return ( value <= RAL_FLRC_CR_2_3 ) ? labels[value] : "Unknown";
}