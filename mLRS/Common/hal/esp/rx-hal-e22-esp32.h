//*******************************************************
// Copyright (c) MLRS project
// GPL3
// https://www.gnu.org/licenses/gpl-3.0.de.html
//*******************************************************
// hal
//********************************************************

//-------------------------------------------------------
// ESP32, E22 32E RX
//-------------------------------------------------------

// #define DEVICE_HAS_NO_LED
#define DEVICE_HAS_SINGLE_LED
//#define DEVICE_HAS_NO_DEBUG
#define DEVICE_HAS_SERIAL_OR_DEBUG
// #define USE_DEBUG


//-- UARTS
// UARTB = serial port
// UARTF = debug port

#define UARTB_USE_SERIAL1
#define UARTB_BAUD                RX_SERIAL_BAUDRATE
#define UARTB_TXBUFSIZE           RX_SERIAL_TXBUFSIZE
#define UARTB_RXBUFSIZE           RX_SERIAL_RXBUFSIZE

#define UARTF_USE_SERIAL
#define UARTF_BAUD                115200


//-- SX1: SX12xx & SPI

// #define SPI_CS_IO                 IO_P18
// #define SPI_FREQUENCY             10000000L
// #define SPI_MISO                  IO_P19
// #define SPI_MOSI                  IO_P27
// #define SPI_SCK                   IO_P5
// #define SX_RESET                  IO_P14
// #define SX_DIO0                   IO_P26

#define SPI_CS_IO                 IO_P18
#define SPI_FREQUENCY             10000000L
#define SPI_MISO                  IO_P19
#define SPI_MOSI                  IO_P27
#define SPI_SCK                   IO_P5

#define SX_RESET                  IO_P14
#define SX_DIO1                   IO_P26
#define SX_BUSY                   IO_P17
#define SX_RX_EN                  IO_P21
#define SX_TX_EN                  IO_P4

IRQHANDLER(void SX_DIO_EXTI_IRQHandler(void);)

void sx_init_gpio(void)
{
    gpio_init(SX_RESET, IO_MODE_OUTPUT_PP_HIGH);
    gpio_init(SX_DIO1, IO_MODE_INPUT_ANALOG);
    gpio_init(SX_BUSY, IO_MODE_INPUT_PU);
    gpio_init(SX_TX_EN, IO_MODE_OUTPUT_PP_LOW);
    gpio_init(SX_RX_EN, IO_MODE_OUTPUT_PP_LOW);
}

IRAM_ATTR bool sx_busy_read(void)
{
    return (gpio_read_activehigh(SX_BUSY)) ? true : false;
}

void sx_amp_transmit(void)
{
    gpio_low(SX_RX_EN);
    gpio_high(SX_TX_EN);
}

void sx_amp_receive(void)
{
    gpio_low(SX_TX_EN);
    gpio_high(SX_RX_EN);
}

void sx_dio_init_exti_isroff(void)
{
    detachInterrupt(SX_DIO1);
}

void sx_dio_enable_exti_isr(void)
{
    attachInterrupt(SX_DIO1, SX_DIO_EXTI_IRQHandler, RISING);
}

IRAM_ATTR void sx_dio_exti_isr_clearflag(void) {}


//-- Button

#define BUTTON                    IO_P0

void button_init(void)
{
    gpio_init(BUTTON, IO_MODE_INPUT_PU);
}

IRAM_ATTR bool button_pressed(void)
{
    return gpio_read_activelow(BUTTON) ? true : false;
}


//-- LEDs

#define LED_RED                   IO_P2

void leds_init(void)
{
    gpio_init(LED_RED, IO_MODE_OUTPUT_PP_LOW);
}

IRAM_ATTR void led_red_off(void) { gpio_low(LED_RED); }
IRAM_ATTR void led_red_on(void) { gpio_high(LED_RED); }
IRAM_ATTR void led_red_toggle(void) { gpio_toggle(LED_RED); }

//-- POWER

// #define POWER_PA_NONE_SX126X
// #define SX_USE_PA_CONFIG_10_DBM
// #include "../hal-power-pa.h"

#define POWER_GAIN_DBM            24 // gain of a PA stage if present
#define POWER_SX126X_MAX_DBM      SX126X_POWER_MAX // maximum allowed sx power
#define POWER_USE_DEFAULT_RFPOWER_CALC

#define RFPOWER_DEFAULT           0 // index into rfpower_list array

const rfpower_t rfpower_list[] = {
    { .dbm = POWER_MIN, .mW = INT8_MIN },
    { .dbm = POWER_0_DBM, .mW = 1 },
    { .dbm = POWER_10_DBM, .mW = 10 },
    { .dbm = POWER_14_DBM, .mW = 25 },
    { .dbm = POWER_20_DBM, .mW = 100 },
    { .dbm = POWER_22_DBM, .mW = 158 },
};