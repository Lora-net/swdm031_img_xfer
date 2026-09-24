/**
 * @file      font_RobotoCondensed_Regular_12.c
 *
 * @brief     Font RobotoCondensed Regular 12
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

/*******************************************************************************
 * Size: 12 px
 * Bpp: 1
 * Opts: --bpp 1 --size 12 --no-compress --stride 1 --align 1 --font
 * RobotoCondensed-Regular.ttf --range 32-127 --format lvgl -o
 * font_RobotoCondensed_Regular_12.c
 ******************************************************************************/

#ifdef __has_include
#if __has_include( "lvgl.h" )
#ifndef LV_LVGL_H_INCLUDE_SIMPLE
#define LV_LVGL_H_INCLUDE_SIMPLE
#endif
#endif
#endif

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef FONT_ROBOTOCONDENSED_REGULAR_12
#define FONT_ROBOTOCONDENSED_REGULAR_12 1
#endif

#if FONT_ROBOTOCONDENSED_REGULAR_12

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xfc, 0x80,

    /* U+0022 "\"" */
    0xb6, 0x80,

    /* U+0023 "#" */
    0x2a, 0xbe, 0xa5, 0x7d, 0x4a, 0xa0,

    /* U+0024 "$" */
    0x10, 0x8c, 0xf4, 0xa0, 0xc1, 0x4b, 0xdc, 0x20,

    /* U+0025 "%" */
    0xe1, 0x52, 0xa7, 0x81, 0x5, 0x94, 0xa9, 0xc,

    /* U+0026 "&" */
    0x72, 0x94, 0xc6, 0x5e, 0x73, 0x78,

    /* U+0027 "'" */
    0xe0,

    /* U+0028 "(" */
    0x29, 0x49, 0x24, 0x91, 0x22,

    /* U+0029 ")" */
    0x89, 0x12, 0x49, 0x25, 0x28,

    /* U+002A "*" */
    0x21, 0x3e, 0xc5, 0x0,

    /* U+002B "+" */
    0x21, 0x9, 0xf2, 0x10, 0x80,

    /* U+002C "," */
    0x54,

    /* U+002D "-" */
    0xe0,

    /* U+002E "." */
    0x80,

    /* U+002F "/" */
    0x11, 0x22, 0x24, 0x44, 0x48,

    /* U+0030 "0" */
    0x74, 0x63, 0x18, 0xc6, 0x33, 0x70,

    /* U+0031 "1" */
    0x3c, 0x92, 0x49, 0x20,

    /* U+0032 "2" */
    0x72, 0x62, 0x11, 0x18, 0x88, 0xf8,

    /* U+0033 "3" */
    0x72, 0x42, 0x33, 0x4, 0x29, 0x70,

    /* U+0034 "4" */
    0x11, 0x8c, 0xa5, 0x4b, 0xe2, 0x10,

    /* U+0035 "5" */
    0x7a, 0x10, 0xe4, 0x86, 0x29, 0x70,

    /* U+0036 "6" */
    0x32, 0x21, 0xe8, 0xc6, 0x39, 0x70,

    /* U+0037 "7" */
    0xf8, 0x44, 0x21, 0x10, 0x84, 0x40,

    /* U+0038 "8" */
    0x74, 0x63, 0xb7, 0x46, 0x31, 0x70,

    /* U+0039 "9" */
    0x74, 0xe3, 0x19, 0xbc, 0x22, 0x60,

    /* U+003A ":" */
    0x82,

    /* U+003B ";" */
    0x87,

    /* U+003C "<" */
    0x13, 0xcc, 0x30,

    /* U+003D "=" */
    0xf0, 0xf,

    /* U+003E ">" */
    0x8e, 0x33, 0xc0,

    /* U+003F "?" */
    0x69, 0x11, 0x26, 0x40, 0x40,

    /* U+0040 "@" */
    0x3c, 0x62, 0x43, 0x99, 0xb9, 0xa9, 0xa9, 0xa9, 0xb6, 0xc0, 0x40, 0x38,

    /* U+0041 "A" */
    0x10, 0xc3, 0x8a, 0x69, 0x27, 0xe1, 0x84,

    /* U+0042 "B" */
    0xf4, 0x63, 0x1f, 0x46, 0x31, 0xf0,

    /* U+0043 "C" */
    0x76, 0x63, 0x8, 0x42, 0x39, 0x70,

    /* U+0044 "D" */
    0xf4, 0xe3, 0x18, 0xc6, 0x33, 0xf0,

    /* U+0045 "E" */
    0xf4, 0x21, 0xf, 0x42, 0x10, 0xf8,

    /* U+0046 "F" */
    0xf8, 0x88, 0xf8, 0x88, 0x80,

    /* U+0047 "G" */
    0x76, 0x61, 0x9, 0xc6, 0x39, 0x70,

    /* U+0048 "H" */
    0x86, 0x18, 0x61, 0xfe, 0x18, 0x61, 0x84,

    /* U+0049 "I" */
    0xff, 0x80,

    /* U+004A "J" */
    0x8, 0x42, 0x10, 0x85, 0x29, 0x70,

    /* U+004B "K" */
    0x8a, 0x4b, 0x28, 0xe2, 0x89, 0x26, 0x88,

    /* U+004C "L" */
    0x88, 0x88, 0x88, 0x88, 0xf0,

    /* U+004D "M" */
    0xc7, 0x8f, 0x1e, 0x3a, 0xb5, 0x6a, 0xc9, 0x92,

    /* U+004E "N" */
    0x87, 0x1c, 0x69, 0xb6, 0x58, 0xe3, 0x84,

    /* U+004F "O" */
    0x7b, 0x38, 0x61, 0x86, 0x18, 0x73, 0x78,

    /* U+0050 "P" */
    0xf4, 0x63, 0x1f, 0x42, 0x10, 0x80,

    /* U+0051 "Q" */
    0x7b, 0x38, 0x61, 0x86, 0x18, 0x73, 0x78, 0x30,

    /* U+0052 "R" */
    0xf4, 0x63, 0x1f, 0x4a, 0x51, 0x88,

    /* U+0053 "S" */
    0x74, 0x63, 0x87, 0xe, 0x31, 0x70,

    /* U+0054 "T" */
    0xf9, 0x8, 0x42, 0x10, 0x84, 0x20,

    /* U+0055 "U" */
    0x8c, 0x63, 0x18, 0xc6, 0x31, 0x70,

    /* U+0056 "V" */
    0xc5, 0x14, 0x52, 0x28, 0xa2, 0x8c, 0x10,

    /* U+0057 "W" */
    0x48, 0xa4, 0xd3, 0x4a, 0xa5, 0x52, 0xa8, 0xd4, 0x44, 0x22, 0x0,

    /* U+0058 "X" */
    0x45, 0x22, 0x8c, 0x10, 0xc2, 0x92, 0x44,

    /* U+0059 "Y" */
    0x8c, 0x54, 0xa3, 0x10, 0x84, 0x20,

    /* U+005A "Z" */
    0xf8, 0x44, 0x42, 0x21, 0x10, 0xf8,

    /* U+005B "[" */
    0xea, 0xaa, 0xaa, 0xc0,

    /* U+005C "\\" */
    0x84, 0x44, 0x22, 0x21, 0x11,

    /* U+005D "]" */
    0xd5, 0x55, 0x55, 0xc0,

    /* U+005E "^" */
    0x22, 0x65, 0x50,

    /* U+005F "_" */
    0xf8,

    /* U+0060 "`" */
    0x4c,

    /* U+0061 "a" */
    0x69, 0x17, 0x99, 0xf0,

    /* U+0062 "b" */
    0x84, 0x21, 0xe8, 0xc6, 0x31, 0x8f, 0x80,

    /* U+0063 "c" */
    0x74, 0xa1, 0x8, 0x49, 0xc0,

    /* U+0064 "d" */
    0x8, 0x42, 0xf8, 0xc6, 0x31, 0x8b, 0xc0,

    /* U+0065 "e" */
    0x74, 0x63, 0xf8, 0x45, 0xc0,

    /* U+0066 "f" */
    0x29, 0x74, 0x92, 0x48,

    /* U+0067 "g" */
    0x7c, 0x63, 0x18, 0xc5, 0xe1, 0xb, 0x80,

    /* U+0068 "h" */
    0x88, 0x8e, 0x99, 0x99, 0x99,

    /* U+0069 "i" */
    0xbf, 0x80,

    /* U+006A "j" */
    0x45, 0x55, 0x57,

    /* U+006B "k" */
    0x88, 0x89, 0xae, 0xea, 0xb9,

    /* U+006C "l" */
    0xff, 0xc0,

    /* U+006D "m" */
    0xed, 0x26, 0x4c, 0x99, 0x32, 0x64, 0x80,

    /* U+006E "n" */
    0xe9, 0x99, 0x99, 0x90,

    /* U+006F "o" */
    0x74, 0xe3, 0x18, 0xe5, 0xc0,

    /* U+0070 "p" */
    0xf4, 0x63, 0x18, 0xc7, 0xd0, 0x84, 0x0,

    /* U+0071 "q" */
    0x7c, 0x63, 0x18, 0xc5, 0xe1, 0x8, 0x40,

    /* U+0072 "r" */
    0xf2, 0x49, 0x20,

    /* U+0073 "s" */
    0x69, 0x86, 0x19, 0x60,

    /* U+0074 "t" */
    0x4b, 0xa4, 0x92, 0x60,

    /* U+0075 "u" */
    0x99, 0x99, 0x99, 0x70,

    /* U+0076 "v" */
    0x8a, 0x94, 0xa5, 0x10, 0x80,

    /* U+0077 "w" */
    0x89, 0x59, 0x5a, 0x56, 0x76, 0x76, 0x26,

    /* U+0078 "x" */
    0x5a, 0x8c, 0x43, 0x29, 0x20,

    /* U+0079 "y" */
    0x8a, 0x94, 0xa5, 0x10, 0x84, 0x22, 0x0,

    /* U+007A "z" */
    0xf1, 0x26, 0x48, 0xf0,

    /* U+007B "{" */
    0x29, 0x24, 0xa2, 0x49, 0x22,

    /* U+007C "|" */
    0xff, 0xe0,

    /* U+007D "}" */
    0x89, 0x24, 0x8a, 0x49, 0x28,

    /* U+007E "~" */
    0xed, 0xc0
};

/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    { .bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0 } /* id = 0 reserved */,
    { .bitmap_index = 0, .adv_w = 44, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0 },
    { .bitmap_index = 1, .adv_w = 47, .box_w = 1, .box_h = 9, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 3, .adv_w = 62, .box_w = 3, .box_h = 3, .ofs_x = 1, .ofs_y = 7 },
    { .bitmap_index = 5, .adv_w = 105, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 11, .adv_w = 95, .box_w = 5, .box_h = 12, .ofs_x = 0, .ofs_y = -1 },
    { .bitmap_index = 19, .adv_w = 122, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 27, .adv_w = 104, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 33, .adv_w = 34, .box_w = 1, .box_h = 3, .ofs_x = 1, .ofs_y = 7 },
    { .bitmap_index = 34, .adv_w = 60, .box_w = 3, .box_h = 13, .ofs_x = 1, .ofs_y = -3 },
    { .bitmap_index = 39, .adv_w = 61, .box_w = 3, .box_h = 13, .ofs_x = 0, .ofs_y = -3 },
    { .bitmap_index = 44, .adv_w = 83, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 4 },
    { .bitmap_index = 48, .adv_w = 95, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 1 },
    { .bitmap_index = 53, .adv_w = 38, .box_w = 2, .box_h = 3, .ofs_x = 0, .ofs_y = -2 },
    { .bitmap_index = 54, .adv_w = 48, .box_w = 3, .box_h = 1, .ofs_x = 0, .ofs_y = 3 },
    { .bitmap_index = 55, .adv_w = 51, .box_w = 1, .box_h = 1, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 56, .adv_w = 71, .box_w = 4, .box_h = 10, .ofs_x = 0, .ofs_y = -1 },
    { .bitmap_index = 61, .adv_w = 95, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 67, .adv_w = 95, .box_w = 3, .box_h = 9, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 71, .adv_w = 95, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0 },
    { .bitmap_index = 77, .adv_w = 95, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0 },
    { .bitmap_index = 83, .adv_w = 95, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 89, .adv_w = 95, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0 },
    { .bitmap_index = 95, .adv_w = 95, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 101, .adv_w = 95, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0 },
    { .bitmap_index = 107, .adv_w = 95, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 113, .adv_w = 95, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 119, .adv_w = 45, .box_w = 1, .box_h = 7, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 120, .adv_w = 39, .box_w = 1, .box_h = 8, .ofs_x = 1, .ofs_y = -1 },
    { .bitmap_index = 121, .adv_w = 86, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 1 },
    { .bitmap_index = 124, .adv_w = 92, .box_w = 4, .box_h = 4, .ofs_x = 1, .ofs_y = 3 },
    { .bitmap_index = 126, .adv_w = 88, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 1 },
    { .bitmap_index = 129, .adv_w = 81, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 134, .adv_w = 148, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = -3 },
    { .bitmap_index = 146, .adv_w = 111, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0 },
    { .bitmap_index = 153, .adv_w = 105, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 159, .adv_w = 109, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 165, .adv_w = 110, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 171, .adv_w = 95, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 177, .adv_w = 92, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 182, .adv_w = 113, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 188, .adv_w = 119, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 195, .adv_w = 48, .box_w = 1, .box_h = 9, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 197, .adv_w = 93, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0 },
    { .bitmap_index = 203, .adv_w = 105, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 210, .adv_w = 91, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 215, .adv_w = 145, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 223, .adv_w = 119, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 230, .adv_w = 115, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 237, .adv_w = 106, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 243, .adv_w = 115, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = -1 },
    { .bitmap_index = 251, .adv_w = 102, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 257, .adv_w = 100, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 263, .adv_w = 100, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 269, .adv_w = 108, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 275, .adv_w = 108, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0 },
    { .bitmap_index = 282, .adv_w = 146, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0 },
    { .bitmap_index = 293, .adv_w = 106, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0 },
    { .bitmap_index = 300, .adv_w = 101, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 306, .adv_w = 100, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 312, .adv_w = 48, .box_w = 2, .box_h = 13, .ofs_x = 1, .ofs_y = -2 },
    { .bitmap_index = 316, .adv_w = 71, .box_w = 4, .box_h = 10, .ofs_x = 0, .ofs_y = -1 },
    { .bitmap_index = 321, .adv_w = 48, .box_w = 2, .box_h = 13, .ofs_x = 0, .ofs_y = -2 },
    { .bitmap_index = 325, .adv_w = 71, .box_w = 4, .box_h = 5, .ofs_x = 0, .ofs_y = 5 },
    { .bitmap_index = 328, .adv_w = 78, .box_w = 5, .box_h = 1, .ofs_x = 0, .ofs_y = -1 },
    { .bitmap_index = 329, .adv_w = 59, .box_w = 3, .box_h = 2, .ofs_x = 0, .ofs_y = 8 },
    { .bitmap_index = 330, .adv_w = 92, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 334, .adv_w = 95, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 341, .adv_w = 89, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 346, .adv_w = 95, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 353, .adv_w = 90, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 358, .adv_w = 61, .box_w = 3, .box_h = 10, .ofs_x = 0, .ofs_y = 0 },
    { .bitmap_index = 362, .adv_w = 95, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = -3 },
    { .bitmap_index = 369, .adv_w = 93, .box_w = 4, .box_h = 10, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 374, .adv_w = 44, .box_w = 1, .box_h = 9, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 376, .adv_w = 43, .box_w = 2, .box_h = 12, .ofs_x = 0, .ofs_y = -3 },
    { .bitmap_index = 379, .adv_w = 87, .box_w = 4, .box_h = 10, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 384, .adv_w = 44, .box_w = 1, .box_h = 10, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 386, .adv_w = 145, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 393, .adv_w = 93, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 397, .adv_w = 96, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 402, .adv_w = 95, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = -3 },
    { .bitmap_index = 409, .adv_w = 96, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = -3 },
    { .bitmap_index = 416, .adv_w = 59, .box_w = 3, .box_h = 7, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 419, .adv_w = 88, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 423, .adv_w = 57, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = 0 },
    { .bitmap_index = 427, .adv_w = 93, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 431, .adv_w = 82, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0 },
    { .bitmap_index = 436, .adv_w = 125, .box_w = 8, .box_h = 7, .ofs_x = 0, .ofs_y = 0 },
    { .bitmap_index = 443, .adv_w = 84, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0 },
    { .bitmap_index = 448, .adv_w = 80, .box_w = 5, .box_h = 10, .ofs_x = 0, .ofs_y = -3 },
    { .bitmap_index = 455, .adv_w = 84, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0 },
    { .bitmap_index = 459, .adv_w = 59, .box_w = 3, .box_h = 13, .ofs_x = 0, .ofs_y = -3 },
    { .bitmap_index = 464, .adv_w = 47, .box_w = 1, .box_h = 11, .ofs_x = 1, .ofs_y = -2 },
    { .bitmap_index = 466, .adv_w = 59, .box_w = 3, .box_h = 13, .ofs_x = 0, .ofs_y = -3 },
    { .bitmap_index = 471, .adv_w = 113, .box_w = 5, .box_h = 2, .ofs_x = 1, .ofs_y = 3 }
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] = { { .range_start       = 32,
                                                  .range_length      = 95,
                                                  .glyph_id_start    = 1,
                                                  .unicode_list      = NULL,
                                                  .glyph_id_ofs_list = NULL,
                                                  .list_length       = 0,
                                                  .type              = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY } };

/*-----------------
 *    KERNING
 *----------------*/

/*Pair left and right glyphs for kerning*/
static const uint8_t kern_pair_glyph_ids[] = { 3, 3, 3, 8, 8, 3, 8, 8, 13, 3, 13, 8, 15, 3, 15, 8, 16, 16 };

/* Kerning between the respective left and right glyphs
 * 4.4 format which needs to scaled with `kern_scale`*/
static const int8_t kern_pair_values[] = { -10, -10, -10, -10, -16, -16, -16, -16, -21 };

/*Collect the kern pair's data in one place*/
static const lv_font_fmt_txt_kern_pair_t kern_pairs = {
    .glyph_ids = kern_pair_glyph_ids, .values = kern_pair_values, .pair_cnt = 9, .glyph_ids_size = 0
};

/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap  = glyph_bitmap,
    .glyph_dsc     = glyph_dsc,
    .cmaps         = cmaps,
    .kern_dsc      = &kern_pairs,
    .kern_scale    = 16,
    .cmap_num      = 1,
    .bpp           = 1,
    .kern_classes  = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif

};

/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t font_RobotoCondensed_Regular_12 = {
#else
lv_font_t font_RobotoCondensed_Regular_12 = {
#endif
    .get_glyph_dsc    = lv_font_get_glyph_dsc_fmt_txt, /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height      = 14,                            /*The maximum line height required by the font*/
    .base_line        = 3,                             /*Baseline measured from the bottom of the line*/
#if !( LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0 )
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK( 7, 4, 0 ) || LVGL_VERSION_MAJOR >= 8
    .underline_position  = -1,
    .underline_thickness = 1,
#endif
    .static_bitmap = 0,
    .dsc           = &font_dsc, /*The custom font data. Will be accessed by
                                   `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK( 8, 2, 0 ) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};

#endif /*#if FONT_ROBOTOCONDENSED_REGULAR_12*/
