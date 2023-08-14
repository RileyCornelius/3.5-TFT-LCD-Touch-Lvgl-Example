#include <Arduino.h>
#include "ui/lvgl_setup.h"

void setup()
{
  Serial.begin(115200);
  lvgl_init();
  benchmark();
}

void loop()
{
  lvgl_handler();
}
