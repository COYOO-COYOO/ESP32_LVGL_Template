#include <Arduino_GFX_Library.h>
#include <lvgl.h>     

#include <btMusicBox.h>
#include "es8388.h"

#include "esp_avrc_api.h"
#include "esp_bt_main.h"

#include "OneButton.h"

/*Set to your screen resolution and rotation*/
#define TFT_HOR_RES   240
#define TFT_VER_RES   240
#define TFT_ROTATION  LV_DISPLAY_ROTATION_0

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];


#define GFX_BL 18                // 背光控制引脚

// 配置 SPI 总线，参数依次为 DC, CS, SCK, MOSI, MISO
Arduino_DataBus *bus = new Arduino_ESP32SPI(4 /* DC */, 5 /* CS */, 14 /* SCK */, 15 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
/*
  col offset 1：列偏移量1，表示显示屏左侧的列偏移量。
  row offset 1：行偏移量1，表示显示屏顶部的行偏移量。
  col offset 2：列偏移量2，表示显示屏右侧的列偏移量。
  row offset 2：行偏移量2，表示显示屏底部的行偏移量。
*/
// 创建 ST7789 显示对象，指定分辨率和偏移量
Arduino_GFX *gfx = new Arduino_ST7789(bus, 23 /* RST */, 0 /* rotation */, true /* IPS */, 240 /* width */, 240 /* height */,
   0 /* col offset 1 */, 0 /* row offset 1 */, 0 /* col offset 2 */, 0 /* row offset 2 */);


void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
    uint32_t w = lv_area_get_width(area);
    uint32_t h = lv_area_get_height(area);

    gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)px_map, w, h);

    lv_display_flush_ready(disp);
}

/*Read the touchpad*/
void my_touchpad_read( lv_indev_t * indev, lv_indev_data_t * data )
{
    /*For example  ("my_..." functions needs to be implemented by you)
    int32_t x, y;
    bool touched = my_get_touch( &x, &y );

    if(!touched) {
        data->state = LV_INDEV_STATE_RELEASED;
    } else {
        data->state = LV_INDEV_STATE_PRESSED;

        data->point.x = x;
        data->point.y = y;
    }
     */
}

/*use Arduinos millis() as tick source*/
static uint32_t my_tick(void)
{
    return millis();
}

void setup()
{
    Serial.begin(115200);
    gfx->begin();
    pinMode(GFX_BL, OUTPUT);
    digitalWrite(GFX_BL, HIGH);

    lv_init();
    lv_tick_set_cb(my_tick);

#if LV_USE_LOG != 0
    lv_log_register_print_cb(my_print);
#endif

    lv_display_t *disp = lv_display_create(TFT_HOR_RES, TFT_VER_RES);
    lv_display_set_flush_cb(disp, my_disp_flush);
    lv_display_set_buffers(disp, draw_buf, NULL, sizeof(draw_buf), LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_rotation(disp, TFT_ROTATION);

    lv_indev_t * indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, my_touchpad_read);

    // 可选择手动创建对象或运行内置 demo
    // lv_demo_widgets();
    // 或手动创建 label:
    lv_obj_t *label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "Hello LVGL 9.2.1!");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    Serial.println("Setup done");
}

void loop()
{
    lv_timer_handler();
    delay(5);
}