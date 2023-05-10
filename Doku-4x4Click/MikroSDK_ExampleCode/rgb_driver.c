/*
 * MikroSDK - MikroE Software Development Kit
 * Copyright© 2020 MikroElektronika d.o.o.
 * 
 * Permission is hereby granted, free of charge, to any person 
 * obtaining a copy of this software and associated documentation 
 * files (the "Software"), to deal in the Software without restriction, 
 * including without limitation the rights to use, copy, modify, merge, 
 * publish, distribute, sublicense, and/or sell copies of the Software, 
 * and to permit persons to whom the Software is furnished to do so, 
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be 
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE 
 * OR OTHER DEALINGS IN THE SOFTWARE. 
 */

/*!
 * \file
 *
 */

#include "c4x4rgb.h"

static void dev_init_diode ( c4x4rgb_t *ctx, uint32_t a_rgb_color, uint8_t diode_num );

static void dev_set_color ( c4x4rgb_t *ctx, uint8_t diode_num );

void c4x4rgb_cfg_setup ( c4x4rgb_cfg_t *cfg, drv_logic_t logic_zero, drv_logic_t logic_one, uint8_t select_ctrl_pin )
{
    // Additional gpio pins
    cfg->in1 = HAL_PIN_NC;
    cfg->in2 = HAL_PIN_NC;
    cfg->out = HAL_PIN_NC;
    
    cfg->logic_zero = logic_zero;
    cfg->logic_one = logic_one;
    
    cfg->ctrl_pin = select_ctrl_pin;
}

err_t c4x4rgb_init ( c4x4rgb_t *ctx, c4x4rgb_cfg_t *cfg ) 
{
    // Output pins 
    digital_out_init( &ctx->in1, cfg->in1 );
    digital_out_init( &ctx->in2, cfg->in2 );
    digital_out_init( &ctx->out, cfg->out );
    
    ctx->logic_zero = cfg->logic_zero;
    ctx->logic_one = cfg->logic_one;
    
    if ( C4X4RGB_CTRL_PIN_IN2 == cfg->ctrl_pin )
    {
        ctx->ctrl_pin = ctx->in2;
    }
    else
    {
        ctx->ctrl_pin = ctx->in1;
    }
    
    return C4X4RGB_OK;
}

err_t c4x4rgb_set_diode ( c4x4rgb_t *ctx, uint32_t diode_num, uint32_t diode_color )
{
    if ( diode_num > 16 )
    {
        return C4X4RGB_ERROR;
    }
    for ( uint8_t cnt_i = 0; cnt_i < 16; cnt_i++ )
    {
        if ( ( diode_num - 1 ) == cnt_i )
        {
            dev_init_diode( ctx, diode_color, cnt_i );
            dev_set_color( ctx, cnt_i );
        }
        else
        {
            dev_set_color( ctx, cnt_i );
        }
    }
    Delay_60us( ); // reset delay
    return C4X4RGB_OK;
}

void c4x4rgb_fill_screen ( c4x4rgb_t *ctx, uint32_t fill_color )
{
    for ( uint8_t cnt_i = 0 ; cnt_i < 16; cnt_i++ )
    {
        c4x4rgb_set_diode( ctx, cnt_i + 1, fill_color );
        Delay_5ms ( );
    }
}

static void dev_init_diode ( c4x4rgb_t *ctx, uint32_t a_rgb_color, uint8_t diode_num )
{
    ctx->diode_array[ diode_num ] = ( a_rgb_color & 0x000000FF ) | 
                                    ( ( a_rgb_color & 0x00FF0000 ) >> 8 ) | 
                                    ( ( a_rgb_color & 0x0000FF00 ) << 8 );
}

static void dev_set_color ( c4x4rgb_t *ctx, uint8_t diode_num )
{
    uint32_t tmp = 1;
    for ( uint8_t cnt_i = 0; cnt_i < 24; cnt_i++ )
    {
        if ( ctx->diode_array[ diode_num ] & ( 1ul << ( 23 - cnt_i ) ) )
        {
            ctx->logic_one( );
        }
        else
        {
            ctx->logic_zero( );
        }
    }
}


// ------------------------------------------------------------------------- END

