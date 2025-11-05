#include "doggo.h"

const int DOGGO_RESOURCE_IDS[12] = {
    RESOURCE_ID_KAGI_DOGGO_12,
    RESOURCE_ID_KAGI_DOGGO_01,
    RESOURCE_ID_KAGI_DOGGO_02,
    RESOURCE_ID_KAGI_DOGGO_03,
    RESOURCE_ID_KAGI_DOGGO_04,
    RESOURCE_ID_KAGI_DOGGO_05,
    RESOURCE_ID_KAGI_DOGGO_06,
    RESOURCE_ID_KAGI_DOGGO_07,
    RESOURCE_ID_KAGI_DOGGO_08,
    RESOURCE_ID_KAGI_DOGGO_09,
    RESOURCE_ID_KAGI_DOGGO_10,
    RESOURCE_ID_KAGI_DOGGO_11,
};

Layer *kagi_doggo_hour;
Layer *doggo_motion_layer;

BitmapLayer *kagi_doggo_bitmap_layer;

GBitmap *kagi_doggo_bitmap;

/**
 * Perform update actions required for doggo motion lines
 * @param context Content with which to draw motion lines with
 * */
void update_doggo_motion_lines(GContext *context)
{
    // Calculate doggo angle based on hour
    uint16_t doggo_angle = ((get_current_hour() % 12) * 30) % 360;

    // Draw dashed trails around the doggo
    uint8_t starting_angles[][2] = {{45, 45}, {240, 60}};
    for (uint8_t i = 0; i < 2; i++)
    {
        draw_dashed_arc_trail(context, DISPLAY_CENTER_X, DISPLAY_CENTER_Y,
                              DOGGO_SIZE / 2 + 6, (doggo_angle + starting_angles[i][0]) % 360, starting_angles[i][1]);
    }
}

/** Perform update actions required for doggo */
void update_doggo()
{
    update_bitmap(&kagi_doggo_bitmap, DOGGO_RESOURCE_IDS[get_current_hour() % 12]);
    layer_mark_dirty(doggo_motion_layer);
}

/**
 * Draw the doggo motion trail
 * @param layer Layer to update
 * @param context Graphics context to use during update
 */
static void layer_update_doggo_motion(Layer *layer, GContext *context)
{
    update_doggo_motion_lines(context);
}

/**
 * Perform actions to load doggo layers onto the given layer
 * @param layer Layer onto which to load doggo layers
 */
void load_doggo_layers(Layer *layer)
{
    GRect bounds = layer_get_bounds(layer);

    kagi_doggo_hour = layer_create(bounds);
    doggo_motion_layer = layer_create(bounds);

    layer_set_update_proc(doggo_motion_layer, layer_update_doggo_motion);

    kagi_doggo_bitmap_layer = create_bitmap_layer(GRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT), kagi_doggo_bitmap);

    bitmap_layer_add_to_layer(kagi_doggo_bitmap_layer, kagi_doggo_hour);

    layer_add_to_layer(kagi_doggo_hour, layer);
    layer_add_to_layer(doggo_motion_layer, layer);
}

/** Perform actions to remove doggo layers from view */
void unload_doggo_layers()
{
    bitmap_layer_destroy_safe(kagi_doggo_bitmap_layer);
    layer_destroy_safe(kagi_doggo_hour);
    layer_destroy_safe(doggo_motion_layer);
}

/** Initialize doggo properties */
void init_doggo()
{
    kagi_doggo_bitmap = gbitmap_create_with_resource(DOGGO_RESOURCE_IDS[0]);
}

/** Deinitialize doggo properties */
void deinit_doggo()
{
    gbitmap_destroy_safe(kagi_doggo_bitmap);
}