#pragma once
#include <Arduino.h>

#if CONFIG_IDF_TARGET_ESP32
#define LCD_DATA_0 12
#define LCD_DATA_1 13
#define LCD_DATA_2 26
#define LCD_DATA_3 25
#define LCD_DATA_4 17
#define LCD_DATA_5 16
#define LCD_DATA_6 27
#define LCD_DATA_7 14
#define LCD_WR 4
#define LCD_RD 2
#define LCD_DC 15
#define LCD_CS 5
#define LCD_RST 10
#define LCD_BL 21
#define TS_SDA SDA
#define TS_SCL SCL
#endif

#if CONFIG_IDF_TARGET_ESP32S3
#define LCD_DATA_0 42
#define LCD_DATA_1 41
#define LCD_DATA_2 40
#define LCD_DATA_3 39
#define LCD_DATA_4 38
#define LCD_DATA_5 37
#define LCD_DATA_6 36
#define LCD_DATA_7 35
#define LCD_WR 4
#define LCD_RD 2
#define LCD_DC 15 // RS
#define LCD_CS 5
#define LCD_RST 17
#define LCD_BL 18
#define TS_SDA SDA
#define TS_SCL SCL
#endif

#define BUTTON_NEXT 42
#define BUTTON_PREV 40
#define BUTTON_ENTER 41