#pragma once
#include <Arduino.h>

#if CONFIG_IDF_TARGET_ESP32
#define LCD_MOSI 23
#define LCD_MISO 19
#define LCD_SCK 18
#define LCD_CS 5
#define LCD_RST 10
#define LCD_DC 9
#define LCD_BL 21
#define TS_MOSI LCD_MOSI
#define TS_MISO LCD_MISO
#define TS_SCK LCD_SCK
#define TS_CS 22
#endif

#if CONFIG_IDF_TARGET_ESP32S3
#define LCD_MOSI 7
#define LCD_MISO 4
#define LCD_SCK 6
#define LCD_CS 5
#define LCD_RST 17
#define LCD_DC 16
#define LCD_BL 18
#define TS_MOSI LCD_MOSI
#define TS_MISO LCD_MISO
#define TS_SCK LCD_SCK
#define TS_CS 15
#endif

#define BUTTON_NEXT 42
#define BUTTON_PREV 40
#define BUTTON_ENTER 41