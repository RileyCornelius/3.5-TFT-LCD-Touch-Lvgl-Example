#define LGFX_USE_V1 // Define before #include <LovyanGFX.hpp>
#include <LovyanGFX.hpp>
#include <Arduino.h>
#include <lvgl.h>
#include <SimpleButton.h>
#include "pin_config.h"
#include "lvgl_setup.h"

#define LCD_WIDTH 320
#define LCD_HEIGHT 480

#define BUTTON_SUPPORT 0

/***************************************************************************************************
 * LGFX Setup
 ***************************************************************************************************/

class LGFX : public lgfx::LGFX_Device
{
    lgfx::Panel_ILI9488 _panel_instance;
    lgfx::Bus_Parallel8 _bus_instance;
    lgfx::Light_PWM _light_instance;
    lgfx::Touch_FT5x06 _touch_instance; // FT6236

public:
    LGFX(void)
    {
        {
            auto cfg = _bus_instance.config();      // バス設定用の構造体を取得します。
            cfg.freq_write = 40000000;              // 送信クロック (最大20MHz, 80MHzを整数で割った値に丸められます)
            cfg.pin_wr = LCD_WR;                    // WR を接続しているピン番号
            cfg.pin_rd = LCD_RD;                    // RD を接続しているピン番号
            cfg.pin_rs = LCD_DC;                    // RS(D/C)を接続しているピン番号
            cfg.pin_d0 = LCD_DATA_0;                // D0を接続しているピン番号
            cfg.pin_d1 = LCD_DATA_1;                // D1を接続しているピン番号
            cfg.pin_d2 = LCD_DATA_2;                // D2を接続しているピン番号
            cfg.pin_d3 = LCD_DATA_3;                // D3を接続しているピン番号
            cfg.pin_d4 = LCD_DATA_4;                // D4を接続しているピン番号
            cfg.pin_d5 = LCD_DATA_5;                // D5を接続しているピン番号
            cfg.pin_d6 = LCD_DATA_6;                // D6を接続しているピン番号
            cfg.pin_d7 = LCD_DATA_7;                // D7を接続しているピン番号
            _bus_instance.config(cfg);              // 設定値をバスに反映します。
            _panel_instance.setBus(&_bus_instance); // バスをパネルにセットします。
        }

        {                                        // 表示パネル制御の設定を行います。
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

        {                                        // バックライト制御の設定を行います。（必要なければ削除）
            auto cfg = _light_instance.config(); // バックライト設定用の構造体を取得します。
            cfg.pin_bl = LCD_BL;                 // バックライトが接続されているピン番号
            cfg.invert = false;                  // バックライトの輝度を反転させる場合 true
            cfg.freq = 44100;                    // バックライトのPWM周波数
            cfg.pwm_channel = 7;                 // 使用するPWMのチャンネル番号

            _light_instance.config(cfg);
            _panel_instance.setLight(&_light_instance); // バックライトをパネルにセットします。
        }

        {
            auto cfg = _touch_instance.config();
            cfg.i2c_port = 1;
            cfg.i2c_addr = 0x38; // FT6236 (ChipID 0x38)
            cfg.pin_sda = TS_SDA;
            cfg.pin_scl = TS_SCL;
            cfg.freq = 400000;
            cfg.x_min = 0;
            cfg.x_max = LCD_WIDTH - 1;
            cfg.y_min = 0;
            cfg.y_max = LCD_HEIGHT - 1;

            _touch_instance.config(cfg);
            _panel_instance.setTouch(&_touch_instance);
        }

        setPanel(&_panel_instance); // 使用するパネルをセットします。
    }
};

LGFX tft;

/***************************************************************************************************
 * LVGL Setup
 ***************************************************************************************************/

/* Display flushing */
static void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
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
static void touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
    uint16_t touchX, touchY;
    bool touched = tft.getTouch(&touchX, &touchY);
    if (touched)
    {
        data->state = LV_INDEV_STATE_PR;
        data->point.x = touchX;
        data->point.y = touchY;
        // Serial.println("X = " + String(touchX) + " Y = " + String(touchY));
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }
}

#if BUTTON_SUPPORT
static void buttons_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
    static SimpleButton button_next(BUTTON_NEXT);
    static SimpleButton button_prev(BUTTON_PREV);
    static SimpleButton button_enter(BUTTON_ENTER);

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
#endif

void lvgl_init()
{
    /*Setup LCD*/
    tft.begin();
    tft.setBrightness(255);
    tft.setRotation(3);

    /*Calibrate touch screen*/
    // uint16_t calData[] = {239, 3926, 233, 265, 3856, 3896, 3714, 308};
    // tft.setTouchCalibrate(calData);

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

#if BUTTON_SUPPORT
    /*Initialize the keypad device driver*/
    static lv_indev_drv_t indev_drv_2;
    lv_indev_drv_init(&indev_drv_2);
    indev_drv_2.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv_2.read_cb = buttons_read;
    lv_indev_t *indev = lv_indev_drv_register(&indev_drv_2);
    lv_group_t *g = lv_group_create();
    lv_group_set_default(g);
    lv_indev_set_group(indev, g);
#endif

    Serial.printf("Lvgl v%d.%d.%d initialized\n", lv_version_major(), lv_version_minor(), lv_version_patch());
}

void lvgl_handler()
{
    const uint16_t period = 5;
    static uint32_t last_tick = 0;
    uint32_t tick = millis();
    if (tick - last_tick > period)
    {
        last_tick = tick;
        lv_task_handler();
    }
}