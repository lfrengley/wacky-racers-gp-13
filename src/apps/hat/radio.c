/** @file   radio.c
    @author Lucy Frengley
            Hayden Evans
            Penelope Keeling
            Michael Spurdle
            M. P. Hayes
    @date   8 May 2024
    @brief  TODO: Write a description
*/

#include "radio.h"
#include "nrf24.h"
#include "pio.h"

#define RADIO_CHANNEL 5                 //TODO: Change?
#define RADIO_ADDRESS 0x0228576780LL    //TODO: Change?
#define RADIO_PAYLOAD_SIZE 32

static nrf24_t *NRF;
static char BUFFER[RADIO_PAYLOAD_SIZE + 1];

void init_radio (void) {
    spi_cfg_t spi_cfg =
    {
        .channel = 0,
        .clock_speed_kHz = 1000,
        .cs = RADIO_CSN_PIO,
        .mode = SPI_MODE_0,
        .cs_mode = SPI_CS_MODE_FRAME,
        .bits = 8
    };

    nrf24_cfg_t nrf24_cfg =
    {
        .channel = RADIO_CHANNEL,
        .address = RADIO_ADDRESS,
        .payload_size = RADIO_PAYLOAD_SIZE,
        .ce_pio = RADIO_CE_PIO,
        // If the IRQ pin is not connected, use a value of 0.
        // .irq_pio = 0,
        .irq_pio = RADIO_IRQ_PIO,
        .spi = spi_cfg,
    };

    #ifdef RADIO_ENABLE_PIO
    // Enable radio regulator if present.
    pio_config_set (RADIO_ENABLE_PIO, PIO_OUTPUT_HIGH);
    #endif

    NRF = nrf24_init (&nrf24_cfg);
    if (! NRF)
        panic (LED_ERROR_PIO, 2);
}

void rx_to_tx (void) {
    nrf24_power_down (NRF);
}

bool radio_write_duties (int16_t left, int16_t right) {
    snprintf (BUFFER, sizeof (BUFFER), "%d %d\r\n", left, right);
    if (! nrf24_write (NRF, BUFFER, RADIO_PAYLOAD_SIZE))  {
        pio_output_set (LED_ERROR_PIO, 0);
        return false;
    } else {
        pio_output_set (LED_ERROR_PIO, 1);
        return true;
    }
}

void radio_read_bump (bool *bump) {
    if (nrf24_read (NRF, BUFFER, sizeof (BUFFER))) {
            printf ("RX: %s\n", BUFFER);            
        if (sscanf(BUFFER, "%hd", bump) == 1) {
            printf ("Bump: %3d\n", *bump);
        } else {
            printf("Invalid\n");
        }
    } else {
        printf ("No new bump: %3d\n", *bump);
    }
}