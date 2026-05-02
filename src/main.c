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
  lv_obj_set_style_bg_color(title_cont,lv_color_black(),0);
  
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
  lv_obj_set_size(down_cont,lv_pct(100),lv_pct(85));
  lv_obj_set_pos(down_cont,0,lv_pct(15));
 
  lv_obj_set_flex_flow(down_cont,LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(down_cont,LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_add_flag(down_cont,LV_OBJ_FLAG_SNAPPABLE);
  lv_obj_add_flag(down_cont, LV_OBJ_FLAG_SCROLLABLE);

  lv_obj_set_layout(down_cont, LV_LAYOUT_FLEX);
  lv_obj_set_scrollbar_mode(down_cont, LV_SCROLLBAR_MODE_ON);
  lv_obj_set_scroll_snap_y(down_cont,LV_SCROLL_SNAP_START);

  lv_obj_set_style_bg_opa(down_cont,LV_OPA_COVER,0);
  lv_obj_set_style_bg_color(down_cont,lv_color_black(),LV_STATE_DEFAULT);

  //在下部分容器中加添加容器，每个容器应该是等比例。
  for(int i=0; i<18; i++)
  {
    lv_obj_t *led_cont = lv_obj_create(down_cont);
    lv_obj_remove_style_all(led_cont);
    lv_obj_set_size(led_cont,lv_pct(100),lv_pct(20));
    lv_obj_set_style_bg_opa(led_cont,LV_OPA_COVER,LV_PART_MAIN);
    if(i == 0)
    {
        lv_obj_set_style_bg_color(led_cont,lv_color_white(),LV_STATE_DEFAULT);
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

    lv_label_set_text(led_cont_lable,"QUCIK SETTING");

    led_cont_item[i] = led_cont;
  }

}