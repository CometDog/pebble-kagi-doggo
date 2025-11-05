#pragma once

#include "@pebble-libraries/pbl-display/pbl-display.h"
#include "@pebble-libraries/pbl-math/pbl-math.h"
#include "@pebble-libraries/pebble-assist/pebble-assist.h"
#include "pebble.h"

#define DOGGO_SIZE 100
#define BALL_SIZE 17
#define SCALE (1.0f / 1024.0f)
#ifdef PBL_PLATFORM_EMERY
#define BALL_ORBIT ((DOGGO_SIZE / 2) + 47)
#else
#define BALL_ORBIT ((DOGGO_SIZE / 2) + 19)
#endif

#ifdef PBL_BW
#define BACKGROUND_COLOR_LIGHT GColorWhite
#define DIAL_GLYPH_COLOR_LIGHT GColorBlack
#define ARC_TRAIL_COLOR_LIGHT GColorBlack
#define BACKGROUND_COLOR_DARK GColorBlack
#define DIAL_GLYPH_COLOR_DARK GColorWhite
#define ARC_TRAIL_COLOR_DARK GColorWhite
#else
#define BACKGROUND_COLOR_LIGHT GColorFromHEX(0x9DEBFE)
#define DIAL_GLYPH_COLOR_LIGHT GColorBlack
#define ARC_TRAIL_COLOR_LIGHT GColorBlack
#define BACKGROUND_COLOR_DARK GColorFromHEX(0x4835BC)
#define DIAL_GLYPH_COLOR_DARK GColorWhite
#define ARC_TRAIL_COLOR_DARK GColorWhite
#endif

#define PERSISTENT_SETTINGS_KEY 1

typedef struct PersistentSettings
{
    bool is_dark_theme;
} PersistentSettings;

extern GColor current_background_color;
extern GColor current_dial_glyph_color;
extern GColor current_arc_trail_color;

void set_current_minute(uint8_t new_minute);
void set_current_hour(uint8_t new_hour);
uint8_t get_current_minute();
uint8_t get_current_hour();
BitmapLayer *create_bitmap_layer(GRect bounds, GBitmap *bitmap);
GPoint get_point_on_circle(int16_t center_x, int16_t center_y, uint16_t radius, uint16_t angle);
GPoint get_minute_position(uint8_t minute, uint16_t radius);
void draw_dashed_arc_trail(GContext *context, int16_t center_x, int16_t center_y, uint16_t radius,
                           uint16_t starting_angle, uint16_t trail_length);
void update_bitmap(GBitmap **bitmap, const uint32_t resource_id);
void set_theme(char *theme);
void init_theme();
void save_settings();
void load_settings();