#include <pebble.h>

void start_speech(TextLayer *s_timer_layer, Window *s_main_window);
void pause_resume_speech();
void show_summary(TextLayer *s_timer_layer, TextLayer *s_participant_layer);
void timer_unload();