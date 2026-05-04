/**
 * @file main.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#ifndef _DEFAULT_SOURCE
  #define _DEFAULT_SOURCE /* needed for usleep() */
#endif

#include <stdlib.h>
#include <stdio.h>
#ifdef _MSC_VER
  #include <Windows.h>
#else
  #include <unistd.h>
  #include <pthread.h>
#endif
#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"
#include "lvgl/demos/lv_demos.h"
#include <SDL.h>

#include "hal/hal.h"

/*********************
 *      DEFINES
 *********************/
#define choose 0
/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

 /*static functions*/
static void create_init(void);

//callback_handler
static void scroll_event_handler(lv_event_t *e);
static void scroll_end_event_handler(lv_event_t *e);
static void create_custom_scrollbar(lv_obj_t *main_cont);

#if LV_USE_OS != LV_OS_FREERTOS

int main(int argc, char **argv)
{
  (void)argc; /*Unused*/
  (void)argv; /*Unused*/

  /*Initialize LVGL*/
  lv_init();

  /*Initialize the HAL (display, input devices, tick) for LVGL*/
  sdl_hal_init(320, 480);

  /* Run the default demo */
  /* To try a different demo or example, replace this with one of: */
  /* - lv_demo_benchmark(); */
  /* - lv_demo_stress(); */
  /* - lv_example_label_1(); */
  /* - etc. */
  //lv_demo_widgets();
  create_init();
  //lv_demo_stress();
  while(1) {
    /* Periodically call the lv_task handler.
     * It could be done in a timer interrupt or an OS task too.*/
    uint32_t sleep_time_ms = lv_timer_handler();
    if(sleep_time_ms == LV_NO_TIMER_READY){
	sleep_time_ms =  LV_DEF_REFR_PERIOD;
    }
#ifdef _MSC_VER
    Sleep(sleep_time_ms);
#else
    usleep(sleep_time_ms * 1000);
#endif
  }

  return 0;
}


#endif

/********************** 
 *   STATIC FUNCTIONS
 **********************/

static lv_obj_t *led_cont_item[20] = {0};
static lv_style_t style_scrollbar;

#if choose == 0
static void create_init(void)
{
    //创建一个主容器
    lv_obj_t *main_cont = lv_obj_create(lv_screen_active());
    lv_obj_remove_style_all(main_cont);
    lv_obj_set_size(main_cont, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_opa(main_cont, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(main_cont,lv_color_black(),0);
    lv_obj_add_flag(main_cont,LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(main_cont,LV_OBJ_FLAG_EVENT_BUBBLE);

    //创建一个容器添加一个标题 快捷设置
    lv_obj_t *title_cont = lv_obj_create(main_cont);
    lv_obj_remove_style_all(title_cont);
    lv_obj_set_size(title_cont, lv_pct(100), lv_pct(15));
    lv_obj_set_pos(title_cont,0,0);
    lv_obj_set_style_bg_opa(title_cont,LV_OPA_COVER,0);
    lv_obj_set_style_bg_color(title_cont,lv_color_hex(0x555555),0);
    
    //添加文本
    lv_obj_t *title_lbale = lv_label_create(title_cont);
    lv_obj_align(title_lbale,LV_ALIGN_LEFT_MID,0,0);
    lv_obj_set_style_text_color(title_lbale,lv_color_white(),0);
    lv_obj_set_style_text_font(title_lbale,&lv_font_montserrat_24,LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(title_lbale,LV_TEXT_ALIGN_LEFT,LV_STATE_DEFAULT);
    lv_label_set_text(title_lbale,"QUCIK SETTING");

    //创建一个下半部分的容器
    lv_obj_t *down_cont = lv_obj_create(main_cont);
    lv_obj_remove_style_all(down_cont);
    lv_obj_set_size(down_cont,lv_pct(95),lv_pct(85));
    lv_obj_set_pos(down_cont,0,lv_pct(15));
  
    lv_obj_set_flex_flow(down_cont,LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(down_cont,LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_add_flag(down_cont,LV_OBJ_FLAG_SNAPPABLE);
    lv_obj_add_flag(down_cont, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_layout(down_cont, LV_LAYOUT_FLEX);
    lv_obj_set_scrollbar_mode(down_cont, LV_SCROLLBAR_MODE_ON);
    lv_obj_set_scroll_snap_y(down_cont,LV_SCROLL_SNAP_NONE);

    // lv_obj_set_scroll_dir(down_cont,LV_DIR_VER); //纵向滚动
    // lv_obj_set_style_radius(down_cont,LV_RADIUS_CIRCLE,LV_PART_SCROLLBAR);
    // lv_obj_set_style_width(down_cont,8,LV_PART_SCROLLBAR);
    // lv_obj_set_style_bg_opa(down_cont,LV_OPA_COVER,LV_PART_SCROLLBAR);
    // lv_obj_set_style_bg_color(down_cont,lv_color_hex(0xffff00),LV_PART_SCROLLBAR);
    // lv_obj_set_style_pad_right(down_cont,8,LV_PART_SCROLLBAR);
    // lv_obj_set_style_bg_opa(down_cont,LV_OPA_COVER,0);
    // lv_obj_set_style_bg_color(down_cont,lv_color_black(),LV_STATE_DEFAULT);
    create_custom_scrollbar(main_cont);

    lv_obj_add_event_cb(down_cont,scroll_event_handler,LV_EVENT_SCROLL,NULL);
    lv_obj_add_event_cb(down_cont,scroll_end_event_handler,LV_EVENT_SCROLL_END,NULL);

    //在下部分容器中加添加容器，每个容器应该是等比例。
    for(int i=0; i<18; i++)
    {
      lv_obj_t *led_cont = lv_obj_create(down_cont);
      lv_obj_remove_style_all(led_cont);
      lv_obj_set_size(led_cont,lv_pct(100),lv_pct(20));
      lv_obj_set_style_bg_opa(led_cont,LV_OPA_COVER,LV_PART_MAIN);
      if(i == 0)
      {
          lv_obj_set_style_bg_color(led_cont,lv_color_hex(0xFFCC00),LV_STATE_DEFAULT);
      }
      else
      {
          lv_obj_set_style_bg_color(led_cont,lv_color_black(),LV_STATE_DEFAULT);
      }
      

      lv_obj_t *led_cont_lable = lv_label_create(led_cont);
      lv_obj_align(led_cont_lable,LV_ALIGN_LEFT_MID,0,0);
      lv_obj_set_style_text_align(led_cont_lable,LV_TEXT_ALIGN_LEFT,LV_STATE_DEFAULT);
      lv_obj_set_style_text_font(led_cont_lable,&lv_font_montserrat_24,LV_STATE_DEFAULT);
      
      if(i == 0)
      {
          lv_obj_set_style_text_color(led_cont_lable,lv_color_black(),0);
      }
      else
      {
          lv_obj_set_style_text_color(led_cont_lable,lv_color_white(),0);
      }

      lv_label_set_text(led_cont_lable,"LED SETTING");

      led_cont_item[i] = led_cont;
    }

}


static void scroll_event_handler(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    lv_obj_t *lsit_cont = lv_obj_get_child(obj,0);
    

}

//min = 0. max = 1377;  times = 81. 1377/17=81
static void scroll_end_event_handler(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);

    uint32_t y = lv_obj_get_scroll_y(obj);
    printf("y: %d\n", y);
}
#endif


#if choose == 1
static void create_init(void)
{
    //创建一个主容器
    lv_obj_t *main_cont = lv_obj_create(lv_screen_active());
    lv_obj_remove_style_all(main_cont);
    lv_obj_set_size(main_cont, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_opa(main_cont, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(main_cont,lv_color_black(),0);
    lv_obj_add_flag(main_cont,LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(main_cont,LV_OBJ_FLAG_EVENT_BUBBLE);
    create_custom_scrollbar(main_cont);


}




// 滚动事件回调
static void scroll_event_handler(lv_event_t * e)
{
    lv_obj_t *cont = lv_event_get_target(e);

    // 取出轨道和滑块
    lv_obj_t *bar_track = lv_obj_get_user_data(cont);
    lv_obj_t *bar_knob  = lv_obj_get_user_data(bar_track);

    // 获取滚动信息
    int32_t scroll_y    = lv_obj_get_scroll_y(cont);
    //int32_t scroll_max  = lv_obj_get_scroll_max_y(cont);
    int32_t track_h     = lv_obj_get_height(bar_track);

    // if(scroll_max > 0) 
    // {
    //     // 计算滑块高度（内容越长，滑块越短）
    //     int32_t knob_h = (track_h * track_h) / (scroll_max + track_h);
    //     if(knob_h < 20) knob_h = 20; // 最小高度限制

    //     lv_obj_set_height(bar_knob, knob_h);

    //     // 计算滑块位置
    //     int32_t knob_y = (scroll_y * (track_h - knob_h)) / scroll_max;
    //     lv_obj_set_y(bar_knob, knob_y);
    // }
}

#endif

//创建右侧滑块
static void create_custom_scrollbar(lv_obj_t *main_cont)
{
    // 2. 创建自定义滚动条轨道（放在右边）
    lv_obj_t *bar_track = lv_obj_create(main_cont);
    lv_obj_remove_style_all(bar_track);
    lv_obj_set_size(bar_track, lv_pct(5), lv_pct(85));
    lv_obj_align(bar_track, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    lv_obj_set_style_bg_opa(bar_track, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_bg_color(bar_track, lv_color_white(), LV_PART_MAIN);
    //lv_obj_set_style_radius(bar_track,LV_RADIUS_CIRCLE,LV_PART_MAIN);

    // 3. 创建滑块
    lv_obj_t *bar_knob = lv_obj_create(bar_track);
    lv_obj_remove_style_all(bar_knob);
    lv_obj_set_size(bar_knob, lv_pct(80), lv_pct(40));
    lv_obj_align(bar_knob, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(bar_knob, lv_color_hex(0x444444), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(bar_knob, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_radius(bar_knob,LV_RADIUS_CIRCLE,LV_PART_MAIN);


    // // 把轨道和滑块保存到容器的 user_data 里，方便回调里用
    // lv_obj_set_user_data(cont, bar_track);
    // lv_obj_set_user_data(bar_track, bar_knob);

    // // 4. 绑定滚动事件
    // lv_obj_add_event_cb(cont, scroll_event_handler, LV_EVENT_SCROLL, NULL);

}