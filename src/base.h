#include "@pebble-libraries/pbl-display/pbl-display.h"
#include "@pebble-libraries/pbl-math/pbl-math.h"
#include "@pebble-libraries/pebble-assist/pebble-assist.h"
#include "pebble.h"

#define DOGGO_SIZE 100
#define BALL_SIZE 17
#define SCALE (1.0f / 1024.0f)
#ifdef PBL_PLATFORM_EMERY
#define BALL_ORBIT ((DOGGO_SIZE / 2) + 45)
#else
#define BALL_ORBIT ((DOGGO_SIZE / 2) + 18)
#endif

#ifdef PBL_BW
#define BACKGROUND_COLOR GColorWhite
#define DIAL_GLYPH_COLOR GColorBlack
#define ARC_TRAIL_COLOR GColorBlack
#else
#define BACKGROUND_COLOR GColorFromHEX(0x9DEBFE)
#define DIAL_GLYPH_COLOR GColorBlack
#define ARC_TRAIL_COLOR GColorBlack
#endif

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