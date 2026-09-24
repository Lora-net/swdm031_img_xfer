/*!
 * \file    oled_display.c
 *
 * \brief   Handle the display on the OLED
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

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <lvgl.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/logging/log.h>

#include "oled_display.h"
#include "app_flrp_img_xfer.h"

LOG_MODULE_REGISTER( display, LOG_LEVEL_INF );

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

#define DISPLAY_THREAD_STACKSIZE 4096
#define DISPLAY_THREAD_PRIORITY 3
#define DISPLAY_TEXT_MAX_LEN 30

#ifndef MIN
#define MIN( a, b ) ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )
#endif

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

typedef struct
{
    uint8_t row;
    char    text[DISPLAY_TEXT_MAX_LEN];
} display_text_t;

typedef struct
{
    const uint8_t* image_buf;
    uint32_t       size;
} display_image_msg_t;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

LV_FONT_DECLARE( font_RobotoCondensed_Regular_10 );
LV_FONT_DECLARE( font_RobotoCondensed_Regular_12 );

static struct k_thread display_thread;
K_THREAD_STACK_DEFINE( display_stack, DISPLAY_THREAD_STACKSIZE );

static lv_obj_t* text_labels[DISPLAY_MAX_ROW];
static lv_obj_t* screen;
static bool      is_display_text = true;

K_MSGQ_DEFINE( display_text_msgq, sizeof( display_text_t ), 20, 4 );
K_MSGQ_DEFINE( display_image_msgq, sizeof( display_image_msg_t ), 2, 4 );

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

static void display_thread_entry( void* p1, void* p2, void* p3 );
static void process_display_text( void );
static void display_frame( const uint8_t* frame_data );
static void create_text_labels( void );
static void cleanup_and_display_image( void );
static void restore_text_mode( void );
static void show_all_frames( const uint8_t* image_buf, uint32_t size );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

void display_thread_init( void )
{
    k_thread_create( &display_thread, display_stack, K_THREAD_STACK_SIZEOF( display_stack ), display_thread_entry, NULL,
                     NULL, NULL, DISPLAY_THREAD_PRIORITY, K_INHERIT_PERMS, K_FOREVER );
    k_thread_start( &display_thread );
}

void display_text( uint8_t row, const char* fmt, ... )
{
    if( row >= DISPLAY_MAX_ROW )
    {
        LOG_ERR( "Row number is out of range" );
        return;
    }

    display_text_t msg;
    msg.row = row;

    va_list args;
    va_start( args, fmt );
    vsnprintf( msg.text, sizeof( msg.text ), fmt, args );
    va_end( args );

    if( k_msgq_put( &display_text_msgq, &msg, K_NO_WAIT ) != 0 )
    {
        LOG_WRN( "Display message queue full, dropping row %u", row );
    }
}

void display_image( const uint8_t* image_buf, uint32_t size )
{
    display_image_msg_t msg = {
        .image_buf = image_buf,
        .size      = size,
    };

    if( k_msgq_put( &display_image_msgq, &msg, K_NO_WAIT ) != 0 )
    {
        LOG_WRN( "Image display queue full" );
    }
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

static void display_thread_entry( void* p1, void* p2, void* p3 )
{
    ARG_UNUSED( p1 );
    ARG_UNUSED( p2 );
    ARG_UNUSED( p3 );

    screen = lv_scr_act( );
    if( screen == NULL )
    {
        LOG_ERR( "Failed to get screen object" );
        return;
    }

    create_text_labels( );

    while( 1 )
    {
        display_image_msg_t image_msg;

        if( k_msgq_get( &display_image_msgq, &image_msg, K_MSEC( is_display_text ? 50 : 10 ) ) == 0 )
        {
            if( is_display_text )
            {
                cleanup_and_display_image( );
            }

            show_all_frames( image_msg.image_buf, image_msg.size );
            k_msleep( 1000 );
            restore_text_mode( );
        }
        else if( is_display_text )
        {
            process_display_text( );
            lv_timer_handler( );
        }
    }
}

static void show_all_frames( const uint8_t* image_buf, uint32_t size )
{
    const uint32_t frame_count = MIN( IMAGE_NUMS, size / SINGLE_IMAGE_SIZE );

    for( uint32_t frame = 0; frame < frame_count; frame++ )
    {
        display_frame( image_buf + frame * SINGLE_IMAGE_SIZE );
    }
}

static void display_frame( const uint8_t* frame_data )
{
    const struct device* display_dev = DEVICE_DT_GET( DT_CHOSEN( zephyr_display ) );

    struct display_buffer_descriptor desc = {
        .width    = 128,
        .height   = 64,
        .buf_size = SINGLE_IMAGE_SIZE,
        .pitch    = 128,
    };

    display_write( display_dev, 0, 0, &desc, frame_data );
}

static void create_text_labels( void )
{
    for( uint8_t i = 0; i < DISPLAY_MAX_ROW; i++ )
    {
        if( text_labels[i] == NULL )
        {
            text_labels[i] = lv_label_create( screen );
            if( i == 0 )
            {
                lv_obj_set_style_text_font( text_labels[i], &font_RobotoCondensed_Regular_12,
                                            LV_PART_MAIN | LV_STATE_DEFAULT );
                lv_obj_align( text_labels[i], LV_ALIGN_TOP_MID, 0, 0 );
            }
            else
            {
                lv_obj_set_style_text_font( text_labels[i], &font_RobotoCondensed_Regular_10,
                                            LV_PART_MAIN | LV_STATE_DEFAULT );
                lv_obj_align( text_labels[i], LV_ALIGN_TOP_LEFT, 0, i * 10 + 4 );
            }

            lv_obj_set_style_text_color( text_labels[i], lv_color_hex( 0x000000 ), LV_PART_MAIN );
            lv_label_set_text( text_labels[i], " " );
        }
    }
}

static void restore_text_mode( void )
{
    if( screen == NULL )
    {
        return;
    }

    create_text_labels( );
    is_display_text = true;
    process_display_text( );
    lv_timer_handler( );
}

static void cleanup_and_display_image( void )
{
    if( screen != NULL )
    {
        lv_obj_clean( screen );
        lv_timer_handler( );

        for( int i = 0; i < DISPLAY_MAX_ROW; i++ )
        {
            text_labels[i] = NULL;
        }

        is_display_text = false;
    }
}

static void process_display_text( void )
{
    display_text_t msg;

    while( k_msgq_get( &display_text_msgq, &msg, K_NO_WAIT ) == 0 )
    {
        if( msg.row < DISPLAY_MAX_ROW && text_labels[msg.row] != NULL )
        {
            lv_label_set_text( text_labels[msg.row], msg.text );
        }
    }
}

/* --- EOF ------------------------------------------------------------------ */
