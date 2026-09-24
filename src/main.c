/**
 * @file      main.c
 *
 * @brief     Application main
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

#include <stdint.h>
#include <stdbool.h>

#include <lvgl.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#include <smtc_modem_hal.h>
#include <smtc_zephyr_usp_api.h>
#include <smtc_sw_platform_helper.h>
#include <smtc_flrp_api.h>
#include "app_flrp_img_xfer.h"
#include "oled_display.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

LOG_MODULE_REGISTER( main, LOG_LEVEL_INF );

#if CONFIG_INITIATOR
extern volatile bool flrp_img_xfer_session_idle_tick;
#endif

#define LED0_NODE DT_ALIAS( led0 )
#if !DT_NODE_HAS_STATUS( LED0_NODE, okay )
#error "Unsupported board: led0 devicetree alias is not defined"
#endif

#define USER_BUTTON_NODE DT_ALIAS( smtc_user_button )
#if !DT_NODE_HAS_STATUS( USER_BUTTON_NODE, okay )
#error "Unsupported board: smtc-user-button devicetree alias is not defined"
#endif

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

static const struct gpio_dt_spec usr_led = GPIO_DT_SPEC_GET( LED0_NODE, gpios );
static const struct gpio_dt_spec button  = GPIO_DT_SPEC_GET_OR( USER_BUTTON_NODE, gpios, { 0 } );

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

static struct gpio_callback button_cb_data;

static volatile bool     user_button_short_press_pending = false;
static volatile bool     button_physically_pressed       = false;
static volatile bool     long_press_handled_this_hold    = false;
static volatile uint32_t button_press_start_ms           = 0;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

static uint8_t anim_completed_count = 0;
static bool    anim_end_flag        = false;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

static void boot_animation( void );
static void animation_completed_cb( lv_anim_t* anim );
static int  configure_user_button( void );
static void user_button_callback( const struct device* dev, struct gpio_callback* cb, uint32_t pins );
#if CONFIG_INITIATOR
static void initiator_show_run_mode( bool periodic_mode );
#endif

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

int main( void )
{
    LOG_INF( "===== FLRP Image Transfer Demo =====" );

    /* Startup logo */
    boot_animation( );
    while( 1 )
    {
        lv_timer_handler( );
        if( anim_end_flag == true )
        {
            // Clear the whole display
            lv_obj_clean( lv_scr_act( ) );
            break;
        }
        k_msleep( 20 );
    }
    /* Boot animation has ended. Display for a while.*/
    k_msleep( 1000 );

    display_thread_init( );
    k_msleep( 10 );

    if( gpio_is_ready_dt( &usr_led ) )
    {
        gpio_pin_configure_dt( &usr_led, GPIO_OUTPUT_ACTIVE );
    }

    if( configure_user_button( ) != 0 )
    {
        LOG_ERR( "Issue when configuring user button, aborting" );
    }

    SMTC_SW_PLATFORM_INIT( );
    SMTC_SW_PLATFORM_VOID( smtc_rac_init( ) );

    if( !flrp_img_xfer_init( ) )
    {
        LOG_ERR( "FLRP init failed" );
        display_text( DISPLAY_ROW4, "Init failed!" );
    }

#if CONFIG_INITIATOR
    uint32_t periodic_timestamp_ms   = smtc_modem_hal_get_time_in_ms( );
    bool     initiator_periodic_mode = true;

    initiator_show_run_mode( initiator_periodic_mode );
#endif

    struct k_sem* event_sem = flrp_img_xfer_get_event_sem( );

    while( true )
    {
#if CONFIG_INITIATOR
        uint32_t now_ms    = smtc_modem_hal_get_time_in_ms( );
        bool     flrp_idle = ( smtc_flrp_call_run( ) == false );

        if( flrp_img_xfer_session_idle_tick )
        {
            flrp_img_xfer_session_idle_tick = false;
            periodic_timestamp_ms           = now_ms;
        }

        if( button_physically_pressed && !long_press_handled_this_hold &&
            ( ( now_ms - button_press_start_ms ) >= FLRP_IMG_XFER_BUTTON_LONG_PRESS_MS ) )
        {
            long_press_handled_this_hold = true;
            initiator_periodic_mode      = !initiator_periodic_mode;
            periodic_timestamp_ms        = now_ms;
            initiator_show_run_mode( initiator_periodic_mode );
            display_text( DISPLAY_ROW6, "Mode switched" );
            LOG_INF( "Run mode: %s", initiator_periodic_mode ? "periodic" : "button" );
        }
        else if( user_button_short_press_pending )
        {
            user_button_short_press_pending = false;
            if( !initiator_periodic_mode )
            {
                flrp_img_xfer_start_transfer( );
            }
        }

        if( initiator_periodic_mode && flrp_idle &&
            ( ( now_ms - periodic_timestamp_ms ) > FLRP_IMG_XFER_INITIATOR_PERIODIC_MS ) )
        {
            flrp_img_xfer_start_transfer( );
        }

        k_sem_take( event_sem, K_MSEC( button_physically_pressed ? 100 : 1000 ) );
#else
        k_sem_take( event_sem, K_MSEC( 1000 ) );
#endif
        gpio_pin_toggle_dt( &usr_led );
    }

    return 0;
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

static void animation_completed_cb( lv_anim_t* anim )
{
    ARG_UNUSED( anim );
    anim_completed_count++;
    if( anim_completed_count >= 2 )
    {
        anim_end_flag = true;
    }
}

static void boot_animation( void )
{
    anim_completed_count = 0;
    anim_end_flag        = false;

    LV_IMG_DECLARE( lora_logo );
    lv_obj_t* img = lv_img_create( lv_scr_act( ) );
    lv_img_set_src( img, &lora_logo );
    lv_obj_align( img, LV_ALIGN_CENTER, 0, 0 );

    uint16_t target_w = lora_logo.header.w;
    uint16_t target_h = lora_logo.header.h;
    lv_obj_set_size( img, target_w * 0.1, target_h * 0.1 );

    lv_anim_t anim_w, anim_h;
    lv_anim_init( &anim_w );
    lv_anim_set_var( &anim_w, img );
    lv_anim_set_exec_cb( &anim_w, ( lv_anim_exec_xcb_t ) lv_obj_set_width );
    lv_anim_set_time( &anim_w, 1000 );                        // animation duration 1s
    lv_anim_set_values( &anim_w, target_w * 0.1, target_w );  // from 10% to 100%
    lv_anim_set_ready_cb( &anim_w, animation_completed_cb );  // End callback
    // height
    lv_anim_init( &anim_h );
    lv_anim_set_var( &anim_h, img );
    lv_anim_set_exec_cb( &anim_h, ( lv_anim_exec_xcb_t ) lv_obj_set_height );
    lv_anim_set_time( &anim_h, 1000 );                        // animation duration 1s
    lv_anim_set_values( &anim_h, target_h * 0.1, target_h );  // from 10% to 100%
    lv_anim_set_ready_cb( &anim_h, animation_completed_cb );  // End callback
    lv_anim_start( &anim_w );
    lv_anim_start( &anim_h );
}

static void user_button_callback( const struct device* dev, struct gpio_callback* cb, uint32_t pins )
{
    ARG_UNUSED( dev );
    ARG_UNUSED( cb );
    ARG_UNUSED( pins );

    uint32_t now_ms = smtc_modem_hal_get_time_in_ms( );

    if( gpio_pin_get_dt( &button ) > 0 )
    {
        button_press_start_ms        = now_ms;
        long_press_handled_this_hold = false;
        button_physically_pressed    = true;
    }
#if CONFIG_INITIATOR
    else
    {
        uint32_t hold_ms = now_ms - button_press_start_ms;

        if( !long_press_handled_this_hold && ( hold_ms >= FLRP_IMG_XFER_BUTTON_DEBOUNCE_MS ) &&
            ( hold_ms < FLRP_IMG_XFER_BUTTON_LONG_PRESS_MS ) )
        {
            user_button_short_press_pending = true;
        }
        button_physically_pressed = false;
    }
#endif

    k_sem_give( flrp_img_xfer_get_event_sem( ) );
}

#if CONFIG_INITIATOR
static void initiator_show_run_mode( bool periodic_mode )
{
    if( periodic_mode )
    {
        display_text( DISPLAY_ROW5, "Mode: Auto (2s idle)" );
    }
    else
    {
        display_text( DISPLAY_ROW5, "Mode: Button" );
    }
    display_text( DISPLAY_ROW4, "Hold btn 2s: switch" );
}
#endif

static int configure_user_button( void )
{
    int ret = 0;

    if( !gpio_is_ready_dt( &button ) )
    {
        printk( "Error: button device %s is not ready\n", button.port->name );
        return 1;
    }

    ret = gpio_pin_configure_dt( &button, GPIO_INPUT );
    if( ret != 0 )
    {
        printk( "Error %d: failed to configure %s pin %d\n", ret, button.port->name, button.pin );
        return 1;
    }

    ret = gpio_pin_interrupt_configure_dt( &button, GPIO_INT_EDGE_BOTH );
    if( ret != 0 )
    {
        printk( "Error %d: failed to configure interrupt on %s pin %d\n", ret, button.port->name, button.pin );
        return 1;
    }

    gpio_init_callback( &button_cb_data, user_button_callback, BIT( button.pin ) );
    gpio_add_callback( button.port, &button_cb_data );

    return 0;
}

/* --- EOF ------------------------------------------------------------------ */
