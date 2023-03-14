
/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#define _DEFAULT_SOURCE /* needed for usleep() */
#include <stdlib.h>
#include <unistd.h>
#define SDL_MAIN_HANDLED /*To fix SDL's "undefined reference to WinMain" issue*/
#include <SDL2/SDL.h>
#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/sdl/sdl.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
enum {
    LV_MENU_ITEM_BUILDER_VARIANT_1,
    LV_MENU_ITEM_BUILDER_VARIANT_2
};
typedef uint8_t lv_menu_builder_variant_t;
/**********************
 *  STATIC PROTOTYPES
 **********************/
static void hal_init(void);
static lv_obj_t * create_text(lv_obj_t * parent, const char * icon, const char * txt,
                              lv_menu_builder_variant_t builder_variant);

static lv_obj_t * create_selectable_text(lv_obj_t * parent, const char * icon, const char * txt,
                                         lv_menu_builder_variant_t builder_variant);

static lv_obj_t * create_slider(lv_obj_t * parent, const char * icon, const char * txt, int32_t min, int32_t max,
                                int32_t val);

static lv_obj_t * create_switch(lv_obj_t * parent, const char * icon, const char * txt, bool chk);

static void reset_wifi_settings_event_handler(lv_event_t * e);
/**********************
 *  STATIC VARIABLES
 **********************/
lv_group_t * default_input_group;
static lv_indev_drv_t indev_drv_3;
lv_indev_t * enc_indev;
lv_obj_t * root_page_k;
/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *      VARIABLES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

void kexample_msgbox_1()
{
    static const char * btns[] = {"Apply", "Close", ""};

    static lv_style_t style1;
    lv_style_init(&style1);

    lv_style_set_bg_opa(&style1, LV_OPA_50);

    lv_obj_t * obj = lv_obj_create(lv_scr_act());
    lv_obj_center(obj);
    lv_obj_set_size(obj,LV_HOR_RES,LV_VER_RES);
    lv_obj_add_style(obj, &style1,0);

    lv_obj_t * mbox1 = lv_msgbox_create(obj, "Hello", "This is a message box with two buttons.", btns, true);

    lv_obj_center(mbox1);
}

void klv_example_menu_5(lv_obj_t * p)
{
    lv_obj_t * menu = lv_menu_create(p);

    lv_color_t bg_color = lv_obj_get_style_bg_color(menu, 0);
    if(lv_color_brightness(bg_color) > 127) {
        lv_obj_set_style_bg_color(menu, lv_color_darken(lv_obj_get_style_bg_color(menu, 0), 50), 0);
    }
    else {
        lv_obj_set_style_bg_color(menu, lv_color_darken(lv_obj_get_style_bg_color(menu, 0), 100), 0);
    }
    lv_menu_set_mode_root_back_btn(menu, LV_MENU_ROOT_BACK_BTN_DISABLED);
    lv_obj_set_size(menu, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL));
    lv_obj_center(menu);

    lv_obj_t * cont;
    lv_obj_t * section;

    /*Create sub pages*/
    lv_obj_t * sub_connection_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_connection_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_connection_page);
    section = lv_menu_section_create(sub_connection_page);

    create_text(section, NULL, "Check below to use Wi-Fi. Uncheck to use Ethernet", LV_MENU_ITEM_BUILDER_VARIANT_1);
    create_switch(section, LV_SYMBOL_WIFI, "Use WiFi", true);
    lv_obj_add_event_cb(
            create_selectable_text(section,LV_SYMBOL_WARNING,"Click to reset wifi remembered network",LV_MENU_ITEM_BUILDER_VARIANT_1),
            reset_wifi_settings_event_handler,LV_EVENT_CLICKED,NULL);


    lv_obj_t * sub_sound_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_sound_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_sound_page);
    section = lv_menu_section_create(sub_sound_page);
    create_switch(section, LV_SYMBOL_AUDIO, "Sound", false);

    lv_obj_t * sub_display_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_display_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_display_page);
    section = lv_menu_section_create(sub_display_page);
    create_slider(section, LV_SYMBOL_SETTINGS, "Brightness", 0, 10, 8);
    create_slider(section, LV_SYMBOL_SETTINGS, "Rotation", 0, 3, 2);

    lv_obj_t * sub_software_info_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_software_info_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    section = lv_menu_section_create(sub_software_info_page);
    create_text(section, NULL, "", LV_MENU_ITEM_BUILDER_VARIANT_1);

    lv_obj_t * sub_legal_info_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_legal_info_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    section = lv_menu_section_create(sub_legal_info_page);
    for(uint32_t i = 0; i < 15; i++) {
        create_text(section, NULL,
                    "This is a long long long long long long long long long text, if it is long enough it may scroll.",
                    LV_MENU_ITEM_BUILDER_VARIANT_1);
    }

    lv_obj_t * sub_about_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_about_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_about_page);
    section = lv_menu_section_create(sub_about_page);
    cont = create_selectable_text(section, NULL, "Software information", LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, cont, sub_software_info_page);
    cont = create_selectable_text(section, NULL, "Legal information", LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, cont, sub_legal_info_page);

    /*Create a root page*/
    root_page_k = lv_menu_page_create(menu, "Settings");
    lv_obj_set_style_pad_hor(root_page_k, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    section = lv_menu_section_create(root_page_k);
    cont = create_selectable_text(section, LV_SYMBOL_SETTINGS, "Connection", LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, cont, sub_connection_page);
    cont = create_selectable_text(section, LV_SYMBOL_AUDIO, "Sound", LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, cont, sub_sound_page);
    cont = create_selectable_text(section, LV_SYMBOL_SETTINGS, "Display", LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, cont, sub_display_page);

    create_text(root_page_k, NULL, "Others", LV_MENU_ITEM_BUILDER_VARIANT_1);
    section = lv_menu_section_create(root_page_k);
    cont = create_selectable_text(section, NULL, "About", LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, cont, sub_about_page);
    create_switch(section, LV_SYMBOL_WIFI, "Use WiFi", true);


    lv_menu_set_sidebar_page(menu, NULL);
    lv_menu_clear_history(menu); /* Clear history because we will be showing the root page later */
    lv_menu_set_page(menu, root_page_k);

    /// this makes the msg box
    kexample_msgbox_1();

}

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

int main(int argc, char **argv)
{
  (void)argc; /*Unused*/
  (void)argv; /*Unused*/

  /*Initialize LVGL*/
  lv_init();

  /*Initialize the HAL (display, input devices, tick) for LVGL*/
  hal_init();

    klv_example_menu_5(lv_scr_act());

  while(1) {
      /* Periodically call the lv_task handler.
       * It could be done in a timer interrupt or an OS task too.*/
      lv_timer_handler();
      usleep(5 * 1000);
  }

  return 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Initialize the Hardware Abstraction Layer (HAL) for the LVGL graphics
 * library
 */
static void hal_init(void)
{
  /* Use the 'monitor' driver which creates window on PC's monitor to simulate a display*/
  sdl_init();

  /*Create a display buffer*/
  static lv_disp_draw_buf_t disp_buf1;
  static lv_color_t buf1_1[SDL_HOR_RES * 100];
  lv_disp_draw_buf_init(&disp_buf1, buf1_1, NULL, SDL_HOR_RES * 100);

  /*Create a display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv); /*Basic initialization*/
  disp_drv.draw_buf = &disp_buf1;
  disp_drv.flush_cb = sdl_display_flush;
  disp_drv.hor_res = SDL_HOR_RES;
  disp_drv.ver_res = SDL_VER_RES;

  lv_disp_t * disp = lv_disp_drv_register(&disp_drv);

  lv_theme_t * th = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), LV_THEME_DEFAULT_DARK, LV_FONT_DEFAULT);
  lv_disp_set_theme(disp, th);

  default_input_group = lv_group_create();
  lv_group_set_default(default_input_group);

  /* Add the mouse as input device
   * Use the 'mouse' driver which reads the PC's mouse*/
//  static lv_indev_drv_t indev_drv_1;
//  lv_indev_drv_init(&indev_drv_1); /*Basic initialization*/
//  indev_drv_1.type = LV_INDEV_TYPE_POINTER;
//
//  /*This function will be called periodically (by the library) to get the mouse position and state*/
//  indev_drv_1.read_cb = sdl_mouse_read;
//  lv_indev_t *mouse_indev = lv_indev_drv_register(&indev_drv_1);

//  static lv_indev_drv_t indev_drv_2;
//  lv_indev_drv_init(&indev_drv_2); /*Basic initialization*/
//  indev_drv_2.type = LV_INDEV_TYPE_KEYPAD;
//  indev_drv_2.read_cb = sdl_keyboard_read;
//  lv_indev_t *kb_indev = lv_indev_drv_register(&indev_drv_2);
//  lv_indev_set_group(kb_indev, g);

    /// only keep this encoder, because ON esp32s3 we will only have encoder
      lv_indev_drv_init(&indev_drv_3); /*Basic initialization*/
      indev_drv_3.type = LV_INDEV_TYPE_ENCODER;
      indev_drv_3.read_cb = sdl_mousewheel_read;
      enc_indev = lv_indev_drv_register(&indev_drv_3);
      lv_indev_set_group(enc_indev, default_input_group);

//  /*Set a cursor for the mouse*/
//  LV_IMG_DECLARE(mouse_cursor_icon); /*Declare the image file.*/
//  lv_obj_t * cursor_obj = lv_img_create(lv_scr_act()); /*Create an image object for the cursor */
//  lv_img_set_src(cursor_obj, &mouse_cursor_icon);           /*Set the image source*/
//  lv_indev_set_cursor(mouse_indev, cursor_obj);             /*Connect the image  object to the driver*/
}

lv_obj_t *create_text(lv_obj_t *parent, const char *icon, const char *txt, lv_menu_builder_variant_t builder_variant) {
    lv_obj_t * obj = lv_menu_cont_create(parent);

    lv_obj_t * img = NULL;
    lv_obj_t * label = NULL;

    if(icon) {
        img = lv_img_create(obj);
        lv_img_set_src(img, icon);
    }

    if(txt) {
        label = lv_label_create(obj);
        lv_label_set_text(label, txt);
        lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_set_flex_grow(label, 1);
    }

    if(builder_variant == LV_MENU_ITEM_BUILDER_VARIANT_2 && icon && txt) {
        lv_obj_add_flag(img, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
        lv_obj_swap(img, label);
    }

    return obj;
}

lv_obj_t *
create_selectable_text(lv_obj_t *parent, const char *icon, const char *txt, lv_menu_builder_variant_t builder_variant) {
    lv_obj_t * obj = lv_menu_cont_create(parent);
    lv_group_add_obj(default_input_group, obj);

    lv_obj_t * img = NULL;
    lv_obj_t * label = NULL;

    if(icon) {
        img = lv_img_create(obj);
        lv_img_set_src(img, icon);
    }

    if(txt) {
        label = lv_label_create(obj);
        lv_label_set_text(label, txt);
        lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_set_flex_grow(label, 1);
    }

    if(builder_variant == LV_MENU_ITEM_BUILDER_VARIANT_2 && icon && txt) {
        lv_obj_add_flag(img, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
        lv_obj_swap(img, label);
    }

    return obj;
}

lv_obj_t *create_slider(lv_obj_t *parent, const char *icon, const char *txt, int32_t min, int32_t max, int32_t val) {
    lv_obj_t * obj = create_text(parent, icon, txt, LV_MENU_ITEM_BUILDER_VARIANT_2);

    lv_obj_t * slider = lv_slider_create(obj);
    lv_obj_set_flex_grow(slider, 1);
    lv_slider_set_range(slider, min, max);
    lv_slider_set_value(slider, val, LV_ANIM_OFF);

    if(icon == NULL) {
        lv_obj_add_flag(slider, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
    }

    return obj;
}

lv_obj_t *create_switch(lv_obj_t *parent, const char *icon, const char *txt, bool chk) {
    lv_obj_t * obj = create_text(parent, icon, txt, LV_MENU_ITEM_BUILDER_VARIANT_1);

    lv_obj_t * sw = lv_switch_create(obj);
    lv_obj_add_state(sw, chk ? LV_STATE_CHECKED : 0);

    return obj;
}

void reset_wifi_settings_event_handler(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) {
        LV_LOG_USER("Clicked");
    }
    else if(code == LV_EVENT_VALUE_CHANGED) {
        LV_LOG_USER("Toggled");
    }
}

