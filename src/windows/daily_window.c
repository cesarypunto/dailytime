#include <pebble.h>
#include "windows/daily_window.h"
#include "timer/timer.h"
  
static Window *s_main_window;
static TextLayer *s_tittle_layer, *s_timer_layer, *s_participant_layer;
static BitmapLayer *s_icon_layer;
int participant = 0;
bool first_time = true;
bool is_summary = false;
Window *my_window;

//text buffer for participants
static char s_participant_text[256] = "Player";

//Action images
static ActionBarLayer *s_action_bar_layer;

static GBitmap *s_icon_bitmap, *s_pause_bitmap, *s_play_bitmap, *s_stop_bitmap;


void show_icon(){
  
  Layer *window_layer = window_get_root_layer(my_window);
  GRect bounds = layer_get_bounds(window_layer);
  
  //Create icon
  s_icon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TITLE_ICON);

  const GEdgeInsets icon_insets = {.top = 65, .right = 28, .bottom = 56, .left = 14};
  s_icon_layer = bitmap_layer_create(grect_inset(bounds, icon_insets));
  bitmap_layer_set_bitmap(s_icon_layer, s_icon_bitmap);
  bitmap_layer_set_compositing_mode(s_icon_layer, GCompOpSet);
  layer_add_child(window_get_root_layer(my_window), bitmap_layer_get_layer(s_icon_layer));
}

void hide_icon()
  {
  bitmap_layer_destroy(s_icon_layer);
}

/**Action handlers**/
//Pause
static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(first_time)
    {
    text_layer_set_text(s_tittle_layer, "KEEP CALM and press down");
  } else if(is_summary)
    {
    text_layer_set_text(s_tittle_layer, "KEEP CALM and press back");
  }
  else{
     pause_resume_speech();
  }
}

//End - Show summary
static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  
  
  if(first_time)
    {
    text_layer_set_text(s_tittle_layer, "KEEP CALM and press down");
  } else if(is_summary)
    {
    text_layer_set_text(s_tittle_layer, "KEEP CALM and press back");
  }
  else{
    hide_icon();
    text_layer_set_text(s_tittle_layer, "Summary");
    show_summary(s_timer_layer, s_participant_layer);
    is_summary = true;

  }
}

//Next
static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(is_summary)
    {
    text_layer_set_text(s_tittle_layer, "KEEP CALM and press back");
  } else {
    if (first_time)
    {
    text_layer_set_text(s_tittle_layer, "Meeting started");
    first_time = false;
    show_icon();
    }
  participant++;
  snprintf(s_participant_text, sizeof(s_participant_text), "Player %d", participant);
  text_layer_set_text(s_participant_layer, s_participant_text);
  start_speech(s_timer_layer, s_main_window);

  }
}

void click_config_provider(void *context) {
 
  window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler) up_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, (ClickHandler) select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, (ClickHandler) down_click_handler);
}

/**End--Action handlers**/


static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  is_summary = false;
  first_time = true;
  
  my_window = window;

  // Create tittle TextLayer

  const GEdgeInsets label_insets_tittle = {.top = 0, .right = ACTION_BAR_WIDTH, .left = ACTION_BAR_WIDTH / 2};
  s_tittle_layer = text_layer_create(grect_inset(bounds, label_insets_tittle));
  text_layer_set_text(s_tittle_layer, "Press down to start meeting");
  text_layer_set_background_color(s_tittle_layer, GColorClear);
  text_layer_set_text_alignment(s_tittle_layer, GTextAlignmentCenter);
  text_layer_set_font(s_tittle_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_tittle_layer));
    
    
  //Create participant layer  
  
  const GEdgeInsets label_insets_participants = {.top = 100, .right = ACTION_BAR_WIDTH, .left = ACTION_BAR_WIDTH / 2};
  s_participant_layer = text_layer_create(grect_inset(bounds, label_insets_participants));
  text_layer_set_text(s_participant_layer, "");
  text_layer_set_background_color(s_participant_layer, GColorClear);
  text_layer_set_text_alignment(s_participant_layer, GTextAlignmentCenter);
  text_layer_set_font(s_participant_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_participant_layer));
  
  //Create timer layer 
  const GEdgeInsets label_insets_timer = {.top = 127, .right = ACTION_BAR_WIDTH, .left = ACTION_BAR_WIDTH / 2};
  s_timer_layer = text_layer_create(grect_inset(bounds, label_insets_timer));
  text_layer_set_text(s_timer_layer, "");
  text_layer_set_background_color(s_timer_layer, GColorClear);
  text_layer_set_text_alignment(s_timer_layer, GTextAlignmentCenter);
  text_layer_set_font(s_timer_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_timer_layer));
    
    
    
   //Create action layer
  s_pause_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PAUSE);
  s_play_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PLAY);
  s_stop_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_STOP);

  
  s_action_bar_layer = action_bar_layer_create();
  action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_UP, s_pause_bitmap);
  action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_DOWN, s_play_bitmap);
  action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_SELECT, s_stop_bitmap);
  action_bar_layer_add_to_window(s_action_bar_layer, window);

  action_bar_layer_set_click_config_provider(s_action_bar_layer,
                                             click_config_provider);   
  

}

static void window_unload(Window *window) {
  //Destroy timer
  timer_unload();
    
  // Destroy tittle TextLayer
  text_layer_destroy(s_tittle_layer);
  //destroy timer layer
  text_layer_destroy(s_timer_layer);
  //destroy participant layer
  text_layer_destroy(s_participant_layer);

  action_bar_layer_destroy(s_action_bar_layer);

  gbitmap_destroy(s_pause_bitmap);
  gbitmap_destroy(s_play_bitmap);
  gbitmap_destroy(s_stop_bitmap);

  participant = 0;
  first_time = true;
  is_summary = false;
  my_window = NULL;
  
  window_destroy(window);
  s_main_window = NULL;
}


void daily_window_push() {
  if(!s_main_window) {
    s_main_window = window_create();
    window_set_window_handlers(s_main_window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
  }
  window_stack_push(s_main_window, true);
}
