#define Nexus_1_0_0_h

#define ST7796_DRIVER

#define SPI_MISO 19
#define SPI_MOSI 23
#define SPI_SCLK 18
#define TFT_MISO SPI_MISO
#define TFT_MOSI SPI_MOSI
#define TFT_SCLK SPI_SCLK
#define TFT_CS 14 // Chip select control pin
#define TFT_DC 25 // Data Command control pin
#define TFT_RST 33
#define TFT_BL 32 // TFT backlight pin
#define TFT_BACKLIGHT_ON HIGH

#define TOUCH_MISO SPI_MISO
#define TOUCH_MOSI SPI_MOSI
#define TOUCH_SCLK SPI_SCLK
#define TOUCH_CS 12 // TouchScreen Chip Select

#define SPI_FREQUENCY 40000000
#define SPI_READ_FREQUENCY 20000000
#define SPI_TOUCH_FREQUENCY 2500000