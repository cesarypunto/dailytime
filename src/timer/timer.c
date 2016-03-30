#include <pebble.h>
#include "timer/timer.h"

int speech_time = 0;
//Main window
static Window *s_window;

bool paused = false;
static int total_time = 0;

//Textlayer for writing result
static TextLayer *s_result_layer;
//text buffer for timer
static char s_timer_text[256] = "0";
//timer pointer
AppTimer *timer;

bool is_first_participant = true;

void timer_handler(void *context) {
      if (!paused)
      {
        speech_time++;
      }
      total_time++;
      
      // wait one second
      if(speech_time<=60){    
        if (paused){
          snprintf(s_timer_text, sizeof(s_timer_text), "Paused [%d]", 60 - speech_time);
        }else{
          if(speech_time == 60)
          {
            vibes_long_pulse();
          }
          snprintf(s_timer_text, sizeof(s_timer_text), "%d", 60 - speech_time); 
        }
      } else {
        if (paused){
          snprintf(s_timer_text, sizeof(s_timer_text), "Paused [%d]", speech_time); 
        }else{
          snprintf(s_timer_text, sizeof(s_timer_text), "%d", speech_time);  
        }
      }
      
      text_layer_set_text(s_result_layer, s_timer_text);  
      timer = app_timer_register(1000, timer_handler, NULL);
}

void start_speech(TextLayer *s_timer_layer, Window *s_main_window) {
  s_window = s_main_window;
  s_result_layer = s_timer_layer;
  
  snprintf(s_timer_text, sizeof(s_timer_text), "60");
  text_layer_set_text(s_result_layer, s_timer_text);
  
  //restart speech time
  speech_time = 0;
  //reset paused
  paused = false;
  
  if (is_first_participant)
    {
    is_first_participant = false;

    app_timer_cancel(timer);
    // Begin counter
    app_timer_register(1000, timer_handler, NULL);  
  } 
  
}


void pause_resume_speech()
{
  paused = !paused;
}

void show_summary(TextLayer *s_timer_layer, TextLayer *s_participant_layer)
{
  int hours, minutes, seconds;
  if(timer!=NULL) 
  {
    //stop the timer
    app_timer_cancel(timer);
    //convert to hours
    hours = (int)(total_time / 3600);  
    minutes = (int)((total_time - hours * 3600) / 60);  
    seconds = total_time - (hours * 3600 + minutes * 60);  
    //printf("%dh %dm %ds",hours,minutes,seconds);
    text_layer_set_text(s_participant_layer, "Duration:"); 
    snprintf(s_timer_text, sizeof(s_timer_text), "%dh %dm %ds",hours,minutes,seconds);
    text_layer_set_text(s_result_layer, s_timer_text);
  }
  else
  {
      printf("No timer");
  }
  
   
}

void timer_unload()
{
  paused = false;
  total_time = 0;
  if(timer!=NULL) 
  {
    app_timer_cancel(timer);
    timer = NULL;
  }
  is_first_participant = true;
}