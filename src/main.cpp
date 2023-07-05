#include <Arduino.h>
#include "ui/lvgl_setup.h"
#include "ui/ui.h"
#include "time_func.h"

void setup()
{
  Serial.begin(115200);
  lvgl_init();
  ui_init();
  time_init();
}

void loop()
{
  lvgl_handler();
  time_handler();
}
