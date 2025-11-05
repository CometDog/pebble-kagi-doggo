#include "ball.h"

const int BALL_RESROUCE_IDS[4] = {
    RESOURCE_ID_KAGI_BALL_01,
    RESOURCE_ID_KAGI_BALL_02,
    RESOURCE_ID_KAGI_BALL_03,
    RESOURCE_ID_KAGI_BALL_04,
};

Layer *kagi_doggo_minute;
Layer *ball_trail_layer;

BitmapLayer *kagi_ball_bitmap_layer;

GBitmap *kagi_ball_bitmap;

/**
 * Perform update actions required for ball motion line
 * @param context Content with which to draw motion line with
 */
void update_ball_trail(GContext *context)
{
    uint16_t ball_angle = (get_current_minute() * 6 + 270) % 360;
    draw_dashed_arc_trail(context, DISPLAY_CENTER_X, DISPLAY_CENTER_Y, BALL_ORBIT, ball_angle, 90);
}

/** Perform update actions required for ball */
void update_ball()
{
    update_bitmap(&kagi_ball_bitmap, BALL_RESROUCE_IDS[get_current_minute() % 4]);
    layer_mark_dirty(kagi_doggo_minute);
    layer_mark_dirty(ball_trail_layer);
}

/**
 * Draw the ball trail
 * @param layer Layer to update
 * @param context Graphics context to use during update
 */
void layer_update_ball_trail(Layer *layer, GContext *context)
{
    update_ball_trail(context);
}

/**
 * Update the ball/minute hand position
 * @param layer Layer to update
 * @param context Graphics context to use during update. Unused
 */
void layer_update_ball_position(Layer *layer, GContext *_)
{
    GPoint ball_pos = get_minute_position(get_current_minute(), BALL_ORBIT);
    layer_set_bounds(layer, GRect(ball_pos.x - (BALL_SIZE / 2), ball_pos.y - (BALL_SIZE / 2), BALL_SIZE, BALL_SIZE));
}

/**
 * Perform actions to load ball layers onto the given layer
 * @param layer Layer onto which to load ball layers
 */
void load_ball_layers(Layer *layer)
{
    GRect bounds = layer_get_bounds(layer);

    ball_trail_layer = layer_create(bounds);
    kagi_doggo_minute = layer_create(bounds);

    layer_set_update_proc(ball_trail_layer, layer_update_ball_trail);
    layer_set_update_proc(kagi_doggo_minute, layer_update_ball_position);

    kagi_ball_bitmap_layer = create_bitmap_layer(GRect(0, 0, BALL_SIZE, BALL_SIZE), kagi_ball_bitmap);

    bitmap_layer_add_to_layer(kagi_ball_bitmap_layer, kagi_doggo_minute);

    layer_add_to_layer(ball_trail_layer, layer);
    layer_add_to_layer(kagi_doggo_minute, layer);
}

/** Perform actions to remove ball layers from view */
void unload_ball_layers()
{
    bitmap_layer_destroy_safe(kagi_ball_bitmap_layer);
    layer_destroy_safe(ball_trail_layer);
    layer_destroy_safe(kagi_doggo_minute);
}

/** Initialize ball properties */
void init_ball()
{
    kagi_ball_bitmap = gbitmap_create_with_resource(BALL_RESROUCE_IDS[0]);
}

/** Deinitialize ball properties */
void deinit_ball()
{
    gbitmap_destroy_safe(kagi_ball_bitmap);
}