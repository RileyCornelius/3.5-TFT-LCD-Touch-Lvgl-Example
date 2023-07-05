#include <Arduino.h>
#include <SimpleButton.h>
#include <lvgl.h>
#define LGFX_USE_V1 // Define before #include <LovyanGFX.hpp>
#include <LovyanGFX.hpp>

#define LCD_WIDTH 320
#define LCD_HEIGHT 480

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

/***************************************************************************************************
 * LGFX Setup
 ***************************************************************************************************/

class LGFX : public lgfx::LGFX_Device
{
  lgfx::Panel_ILI9488 _panel_instance;
  lgfx::Bus_SPI _bus_instance;
  lgfx::Light_PWM _light_instance;
  lgfx::Touch_XPT2046 _touch_instance;

public:
  LGFX(void)
  {
    {                                         // バス制御の設定を行います。
      auto cfg = _bus_instance.config();      // バス設定用の構造体を取得します。
      cfg.spi_host = SPI3_HOST;               // 使用するSPIを選択  (VSPI_HOST or HSPI_HOST)
      cfg.spi_mode = 0;                       // SPI通信モードを設定 (0 ~ 3)
      cfg.freq_write = 40000000;              // 送信時のSPIクロック (最大80MHz, 80MHzを整数で割った値に丸められます)
      cfg.freq_read = 16000000;               // 受信時のSPIクロック
      cfg.spi_3wire = false;                  // 受信をMOSIピンで行う場合はtrueを設定
      cfg.use_lock = true;                    // トランザクションロックを使用する場合はtrueを設定
      cfg.dma_channel = 1;                    // Set the DMA channel (1 or 2. 0=disable)   使用するDMAチャンネルを設定 (0=DMA不使用)
      cfg.pin_sclk = LCD_SCK;                 // SPIのSCLKピン番号を設定
      cfg.pin_mosi = LCD_MOSI;                // SPIのMOSIピン番号を設定
      cfg.pin_miso = LCD_MISO;                // SPIのMISOピン番号を設定 (-1 = disable)
      cfg.pin_dc = LCD_DC;                    // SPIのD/Cピン番号を設定  (-1 = disable)
      _bus_instance.config(cfg);              // 設定値をバスに反映します。
      _panel_instance.setBus(&_bus_instance); // バスをパネルにセットします。
    }

    {                                      // 表示パネル制御の設定を行います。
      auto cfg = _panel_instance.config(); // 表示パネル設定用の構造体を取得します。
      cfg.pin_cs = LCD_CS;                 // CSが接続されているピン番号   (-1 = disable)
      cfg.pin_rst = LCD_RST;               // RSTが接続されているピン番号  (-1 = disable)
      cfg.pin_busy = -1;                   // BUSYが接続されているピン番号 (-1 = disable)
      cfg.memory_width = LCD_WIDTH;        // ドライバICがサポートしている最大の幅
      cfg.memory_height = LCD_HEIGHT;      // ドライバICがサポートしている最大の高さ
      cfg.panel_width = LCD_WIDTH;         // 実際に表示可能な幅
      cfg.panel_height = LCD_HEIGHT;       // 実際に表示可能な高さ
      cfg.offset_x = 0;                    // パネルのX方向オフセット量
      cfg.offset_y = 0;                    // パネルのY方向オフセット量
      cfg.offset_rotation = 0;             // 回転方向の値のオフセット 0~7 (4~7は上下反転)
      cfg.dummy_read_pixel = 8;            // ピクセル読出し前のダミーリードのビット数
      cfg.dummy_read_bits = 1;             // ピクセル以外のデータ読出し前のダミーリードのビット数
      cfg.readable = true;                 // データ読出しが可能な場合 trueに設定
      cfg.invert = false;                  // パネルの明暗が反転してしまう場合 trueに設定
      cfg.rgb_order = false;               // パネルの赤と青が入れ替わってしまう場合 trueに設定
      cfg.dlen_16bit = false;              // データ長を16bit単位で送信するパネルの場合 trueに設定
      cfg.bus_shared = true;               // SDカードとバスを共有している場合 trueに設定(drawJpgFile等でバス制御を行います)

      _panel_instance.config(cfg);
    }

    {                                      // バックライト制御の設定を行います。（必要なければ削除）
      auto cfg = _light_instance.config(); // バックライト設定用の構造体を取得します。

      cfg.pin_bl = LCD_BL; // バックライトが接続されているピン番号
      cfg.invert = false;  // バックライトの輝度を反転させる場合 true
      cfg.freq = 44100;    // バックライトのPWM周波数
      cfg.pwm_channel = 7; // 使用するPWMのチャンネル番号

      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance); // バックライトをパネルにセットします。
    }

    { // タッチスクリーン制御の設定を行います。（必要なければ削除）
      auto cfg = _touch_instance.config();
      cfg.x_min = 0;              // タッチスクリーンから得られる最小のX値(生の値)
      cfg.x_max = LCD_WIDTH - 1;  // タッチスクリーンから得られる最大のX値(生の値)
      cfg.y_min = 0;              // タッチスクリーンから得られる最小のY値(生の値)
      cfg.y_max = LCD_HEIGHT - 1; // タッチスクリーンから得られる最大のY値(生の値)
      cfg.pin_int = -1;           // INTが接続されているピン番号
      cfg.bus_shared = true;      // 画面と共通のバスを使用している場合 trueを設定
      cfg.offset_rotation = 0;    // 表示とタッチの向きのが一致しない場合の調整 0~7の値で設定
      cfg.spi_host = SPI3_HOST;   // 使用するSPIを選択 (HSPI_HOST or VSPI_HOST)
      cfg.freq = 1000000;         // SPIクロックを設定
      cfg.pin_sclk = TS_SCK;      // SCLKが接続されているピン番号
      cfg.pin_mosi = TS_MOSI;     // MOSIが接続されているピン番号
      cfg.pin_miso = TS_MISO;     // MISOが接続されているピン番号
      cfg.pin_cs = TS_CS;         //   CSが接続されているピン番号
      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance); // タッチスクリーンをパネルにセットします。
    }
    setPanel(&_panel_instance); // 使用するパネルをセットします。
  }
};

LGFX tft;

/***************************************************************************************************
 * LVGL Setup
 ***************************************************************************************************/

/* Display flushing */
void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  // tft.pushColors((uint16_t *)&color_p->full, w * h, true);
  tft.writePixels((lgfx::rgb565_t *)&color_p->full, w * h);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

/*Read the touchpad*/
void touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  uint16_t touchX, touchY;
  bool touched = tft.getTouch(&touchX, &touchY);
  if (!touched)
  {
    data->state = LV_INDEV_STATE_REL;
  }
  else
  {
    data->state = LV_INDEV_STATE_PR;

    /*Set the coordinates*/
    data->point.x = touchX;
    data->point.y = touchY;

    // Serial.println("X = " + String(touchX) + " Y = " + String(touchY));
  }
}

static void buttons_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  static SimpleButton button_next(42);
  static SimpleButton button_prev(40);
  static SimpleButton button_enter(41);

  if (button_next.pressed())
  {
    Serial.println("Next");
    data->key = LV_KEY_NEXT;
    data->state = LV_INDEV_STATE_PRESSED;
  }
  else if (button_prev.pressed())
  {
    Serial.println("Prev");
    data->key = LV_KEY_PREV;
    data->state = LV_INDEV_STATE_PRESSED;
  }
  else if (button_enter.pressed())
  {
    Serial.println("Enter");
    data->key = LV_KEY_ENTER;
    data->state = LV_INDEV_STATE_PRESSED;
  }
  else
  {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

void lvgl_init()
{
  /*Setup LCD*/
  tft.begin();
  tft.setBrightness(255);
  tft.setRotation(3);

  /*Calibrate touch screen*/
  uint16_t calData[] = {239, 3926, 233, 265, 3856, 3896, 3714, 308};
  tft.setTouchCalibrate(calData);

  /*Initialize the buffer*/
  static lv_disp_draw_buf_t draw_buf;
  static lv_color_t buf[LCD_HEIGHT * LCD_WIDTH / 10];
  lv_init();
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, LCD_HEIGHT * LCD_WIDTH / 10);

  /*Initialize the display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);

  /*Change the following line to your display resolution*/
  disp_drv.hor_res = LCD_HEIGHT;
  disp_drv.ver_res = LCD_WIDTH;
  disp_drv.flush_cb = display_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /*Initialize the touch device driver*/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = touchpad_read;
  lv_indev_drv_register(&indev_drv);

  /*Initialize the keypad device driver*/
  static lv_indev_drv_t indev_drv_2;
  lv_indev_drv_init(&indev_drv_2);
  indev_drv_2.type = LV_INDEV_TYPE_KEYPAD;
  indev_drv_2.read_cb = buttons_read;
  lv_indev_t *indev = lv_indev_drv_register(&indev_drv_2);
  lv_group_t *g = lv_group_create();
  lv_group_set_default(g);
  lv_indev_set_group(indev, g);

  Serial.printf("Lvgl v%d.%d.%d initialized\n", lv_version_major(), lv_version_minor(), lv_version_patch());
}

/***************************************************************************************************
 * UI
 ***************************************************************************************************/

static void btn_event_cb(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *btn = lv_event_get_target(e);
  if (code == LV_EVENT_CLICKED)
  {
    static uint8_t cnt = 0;
    cnt++;

    /*Get the first child of the button which is the label and change its text*/
    lv_obj_t *label = lv_obj_get_child(btn, 0);
    lv_label_set_text_fmt(label, "Button: %d", cnt);
  }
}

void lv_example_button(void)
{
  lv_obj_t *btn = lv_btn_create(lv_scr_act()); /*Add a button the current screen*/
  lv_obj_set_size(btn, 120, 50);               /*Set its size*/
  lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);
  lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL); /*Assign a callback to the button*/

  lv_obj_t *label = lv_label_create(btn); /*Add a label to the button*/
  lv_label_set_text(label, "Button");     /*Set the labels text*/
  lv_obj_center(label);

  lv_obj_t *btn2 = lv_btn_create(lv_scr_act()); /*Add a button the current screen*/
  lv_obj_set_size(btn2, 120, 50);               /*Set its size*/
  lv_obj_align(btn2, LV_ALIGN_CENTER, 0, 60);
  lv_obj_add_event_cb(btn2, btn_event_cb, LV_EVENT_ALL, NULL); /*Assign a callback to the button*/

  lv_obj_t *label2 = lv_label_create(btn2); /*Add a label to the button*/
  lv_label_set_text(label2, "Button");      /*Set the labels text*/
  lv_obj_center(label2);
}

/***************************************************************************************************
 * Arduino
 ***************************************************************************************************/

void setup()
{
  Serial.begin(115200);
  lvgl_init();
  lv_example_button();
}

void loop()
{
  lv_timer_handler(); /* let the GUI do its work */
  delay(5);
}
