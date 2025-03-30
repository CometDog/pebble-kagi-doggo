#include "pebble.h"
#include "@pebble-libraries/pebble-assist/pebble-assist.h"
#include "@pebble-libraries/pbl-math/pbl-math.h"
#include "@pebble-libraries/pbl-display/pbl-display.h"

#define DOGGO_SIZE 100
#define BALL_SIZE 15
#define SCALE (1.0f / 1024.0f)
#define BALL_ORBIT ((DOGGO_SIZE / 2) + BALL_SIZE + 4)

void set_current_minute(uint8_t new_minute);
void set_current_hour(uint8_t new_hour);
uint8_t get_current_minute();
uint8_t get_current_hour();
BitmapLayer *create_bitmap_layer(GRect bounds, GBitmap *bitmap);
GPoint get_point_on_circle(int16_t center_x, int16_t center_y, uint16_t radius, uint16_t angle);
void draw_dashed_arc_trail(GContext *context, int16_t center_x, int16_t center_y,
                           uint16_t radius, uint16_t starting_angle, uint16_t trail_length);
void update_bitmap(GBitmap **bitmap, const uint32_t resource_id);