#include "main.h"

#define FONT_SIZE 20

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
  uint8_t round_face_positions_buffer[12][2];
  square_face_positions(FONT_SIZE + 4, 0, 0, round_face_positions_buffer);

  graphics_context_set_text_color(context, GColorBlack);

  for (uint8_t i = 2; i < 12; i += 3)
  {
    uint8_t x_coord = round_face_positions_buffer[i][0];
    uint8_t y_coord = round_face_positions_buffer[i][1];

    uint8_t actual_hour = i + 1;
    uint8_t number_height = FONT_SIZE + 4;
    uint8_t number_width = (actual_hour > 9) ? FONT_SIZE * 2 : FONT_SIZE;

    char digits[3];
    pbl_itoa(actual_hour, digits, 10);
    graphics_draw_text(context, digits, kagi_font,
                       GRect(x_coord, y_coord, number_width, number_height),
                       GTextOverflowModeFill, GTextAlignmentCenter, NULL);
  }
}

/**
 * Main window load handler
 * @param window The window being loaded
 */
static void main_window_load(Window *window)
{
  window_set_background_color(window, BACKGROUND_COLOR);
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
 * Initialize the app
 */
static void init(void)
{
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
