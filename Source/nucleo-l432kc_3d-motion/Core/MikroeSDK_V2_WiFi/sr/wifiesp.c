/*!
 * \file
 *
 */
#include "wifiesp.h"
#include "string.h"

// ------------------------------------------------ PUBLIC FUNCTION DEFINITIONS

void wifiesp_cfg_setup ( wifiesp_cfg_t *cfg )
{
    // Communication gpio pins 

    cfg->rx_pin = HAL_PIN_NC;
    cfg->tx_pin = HAL_PIN_NC;
    
    // Additional gpio pins

    cfg->rst = HAL_PIN_NC;
    cfg->cs   = HAL_PIN_NC;

    cfg->baud_rate      = 115200;
    cfg->data_bit       = UART_DATA_BITS_DEFAULT;
    cfg->parity_bit     = UART_PARITY_DEFAULT;
    cfg->stop_bit       = UART_STOP_BITS_DEFAULT;
    cfg->uart_blocking  = false;
}

WIFIESP_RETVAL wifiesp_init ( wifiesp_t *ctx, wifiesp_cfg_t *cfg )
{
    uart_config_t uart_cfg;

    // Default config
    uart_configure_default( &uart_cfg );

    // Ring buffer mapping
    ctx->uart.tx_ring_buffer = ctx->uart_tx_buffer;
    ctx->uart.rx_ring_buffer = ctx->uart_rx_buffer;

    // UART module config
    uart_cfg.rx_pin = cfg->rx_pin;  // UART RX pin. 
    uart_cfg.tx_pin = cfg->tx_pin;  // UART TX pin. 
    uart_cfg.tx_ring_size = sizeof( ctx->uart_tx_buffer );  
    uart_cfg.rx_ring_size = sizeof( ctx->uart_rx_buffer );

    uart_open( &ctx->uart, &uart_cfg );
    uart_set_baud( &ctx->uart, cfg->baud_rate );
    uart_set_parity( &ctx->uart, cfg->parity_bit );
    uart_set_stop_bits( &ctx->uart, cfg->stop_bit );  
    uart_set_data_bits( &ctx->uart, cfg->data_bit );

    uart_set_blocking( &ctx->uart, cfg->uart_blocking );

    // Output pins 

    digital_out_init( &ctx->rst, cfg->rst );
    digital_out_init( &ctx->cs, cfg->cs );
    
    return WIFIESP_OK;
}

void wifiesp_default_cfg ( wifiesp_t *ctx )
{
    Delay_100ms( );
    digital_out_high( &ctx->rst );
    digital_out_high( &ctx->cs );
}

void wifiesp_generic_write ( wifiesp_t *ctx, char *data_buf, uint16_t len )
{
    uart_write( &ctx->uart, data_buf, len );
}

int32_t wifiesp_generic_read ( wifiesp_t *ctx, char *data_buf, int32_t max_len )
{
    return uart_read( &ctx->uart, data_buf, max_len );
}

void wifiesp_send_cmd( wifiesp_t* ctx, uint8_t* cmd, uint8_t* args )
{
    uint8_t tmp_buf[ 50 ] = { 0 };
    uint16_t len;
    
    len = strlen( cmd );
    memcpy( tmp_buf, cmd, len );
    
    if( NULL != args )
    {
        strcat( tmp_buf, "=" );
        strcat( tmp_buf, args );
        len += ( strlen( args ) + 1 );
    }
    strcat( tmp_buf, "\r\n" );
    len += 2;
    wifiesp_generic_write( ctx, tmp_buf, len );
}

// ------------------------------------------------------------------------- END

