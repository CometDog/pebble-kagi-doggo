#include "main.h"

#define FONT_SIZE 20

Window *main_window;
Layer *background;
Layer *dial;
GFont kagi_font;

/**
 * Handle time tick event
 * @param tick_time Pointer to the time structure
 * @param units_changed The units that have changed. Unused
 */
static void tick_handler(struct tm *tick_time, TimeUnits _)
{
    if (tick_time->tm_hour != get_current_hour())
    {
        set_current_hour(tick_time->tm_hour);
        update_doggo();
    }

    if (tick_time->tm_min != get_current_minute())
    {
        set_current_minute(tick_time->tm_min);
        update_ball();
    }
}

/**
 * Update the clock dial
 * @param layer Layer to update. Unused
 * @param context Graphics context to use during update
 */
static void layer_update_dial(Layer *_, GContext *context)
{
    graphics_context_set_text_color(context, current_dial_glyph_color);
    graphics_context_set_fill_color(context, current_dial_glyph_color);

    uint16_t digit_radius = BALL_ORBIT - 5;

    for (uint8_t i = 0; i < 12; i++)
    {
        uint8_t minute_for_hour = (i + 1) * 5;
        GPoint digit_pos = get_minute_position(minute_for_hour, digit_radius);

        uint8_t actual_hour = i + 1;

        uint8_t number_height = FONT_SIZE + 4;
        uint8_t number_width = (actual_hour >= 10) ? FONT_SIZE * 2 : FONT_SIZE;

        int16_t x_coord = digit_pos.x - (number_width / 2);
        int16_t y_coord = digit_pos.y - (number_height / 2);

        char digits[3];
        pbl_itoa(actual_hour, digits, 10);
        if (actual_hour == 12 || actual_hour == 3 || actual_hour == 6 || actual_hour == 9)
        {
            graphics_draw_text(context, digits, kagi_font, GRect(x_coord, y_coord, number_width, number_height),
                               GTextOverflowModeFill, GTextAlignmentCenter, NULL);
        }
        else
        {
            graphics_fill_circle(context, GPoint(digit_pos.x, digit_pos.y), 2);
        }
    }
}

/**
 * Main window load handler
 * @param window The window being loaded
 */
static void main_window_load(Window *window)
{
    window_set_background_color(window, current_background_color);
    GRect bounds = window_get_bounds(window);

    background = layer_create(bounds);

    dial = layer_create(bounds);
    layer_set_update_proc(dial, layer_update_dial);

    layer_add_to_layer(dial, background);
    load_doggo_layers(background);
    load_ball_layers(background);

    layer_add_to_window(background, window);
}

/**
 * Main window unload handler
 * @param window The window being unloaded
 */
static void main_window_unload(Window *window)
{
    unload_doggo_layers();
    unload_ball_layers();
    layer_destroy_safe(dial);
    layer_destroy_safe(background);
}

/**
 * Redraw all assets
 */
void redraw_all_assets()
{
    if (main_window)
    {
        window_set_background_color(main_window, current_background_color);

        if (dial)
        {
            layer_mark_dirty(dial);
        }
        if (background)
        {
            layer_mark_dirty(background);
        }
    }
}

/**
 * Initialize the app
 */
static void init(void)
{
    events_init();
    init_theme();
    init_doggo();
    init_ball();

    kagi_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LUFGA_20));

    main_window = window_create();
    window_handlers(main_window, main_window_load, main_window_unload);
    window_stack_push(main_window, true);

    time_t now = time(NULL);
    struct tm *tick_time = localtime(&now);
    tick_handler(tick_time, MINUTE_UNIT);

    debug_tick_timer_service_subscribe(MINUTE_UNIT, tick_handler, REAL);
}

/**
 * Deinitialize the app
 */
static void deinit(void)
{
    events_deinit();
    deinit_doggo();
    deinit_ball();
    fonts_unload_custom_font(kagi_font);
    debug_tick_timer_service_unsubscribe();
    window_destroy_safe(main_window);
}

/**
 * Main entry point
 */
int main(void)
{
    init();
    app_event_loop();
    deinit();
    return 0;
}
