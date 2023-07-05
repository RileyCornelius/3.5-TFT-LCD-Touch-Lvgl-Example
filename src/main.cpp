#include <Arduino.h>
#include "ui/lvgl_setup.h"
#include "ui/ui.h"

void setup()
{
  Serial.begin(115200);
  lvgl_init();
  ui_init();
}

void loop()
{
  lvgl_handler();
}
