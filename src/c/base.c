#include "base.h"
#include "main.h"

static PersistentSettings settings;

uint16_t current_minute = 0;
uint16_t current_hour = 0;

GColor current_background_color;
GColor current_dial_glyph_color;
GColor current_arc_trail_color;

/**
 * Update current minute
 * @param new_minute The minute to update to
 */
void set_current_minute(uint8_t new_minute)
{
    current_minute = new_minute;
}

/**
 * Update current hour
 * @param new_hour The hour to udpate to
 */
void set_current_hour(uint8_t new_hour)
{
    current_hour = new_hour;
}

/** Get current minute */
uint8_t get_current_minute()
{
    return current_minute;
}

/** Get current hour */
uint8_t get_current_hour()
{
    return current_hour;
}

/**
 * Create and configure a bitmap layer
 * @param bounds Rectangle defining the bounds
 * @param bitmap Initial bitmap to set
 * @return Configured bitmap layer
 */
BitmapLayer *create_bitmap_layer(GRect bounds, GBitmap *bitmap)
{
    BitmapLayer *layer = bitmap_layer_create(bounds);
    bitmap_layer_set_compositing_mode(layer, GCompOpSet);
    bitmap_layer_set_bitmap(layer, bitmap);
    return layer;
}

/**
 * Calculate position on a circle
 * @param center_x X center reference point to circle
 * @param center_y Y center reference point to circle
 * @param radius Radius of the circle
 * @param angle Angle of point to retrieve in degrees
 * @return GPoint with the calculated position
 */
GPoint get_point_on_circle(int16_t center_x, int16_t center_y, uint16_t radius, uint16_t angle)
{
    return GPoint(center_x + (int)(radius * pbl_cos_sin_deg(angle) * SCALE),
                  center_y + (int)(radius * pbl_int_sin_deg(angle) * SCALE));
}

/**
 * Get the position for a given minute on the clock face of a specified radius
 * @param minute The minute (0-59)
 * @param radius The radius from center to the position
 * @return GPoint representing the position
 */
GPoint get_minute_position(uint8_t minute, uint16_t radius)
{
    // Each minute is 6 degrees
    uint16_t angle = (minute * 6 + 270) % 360;
    return get_point_on_circle(DISPLAY_CENTER_X, DISPLAY_CENTER_Y, radius, angle);
}

/**
 * Draw a dashed arc trail
 * @param context Graphics context to use for drawing
 * @param center_x X center reference point to circle
 * @param center_y Y center reference point to circle
 * @param radius Radius of the arc
 * @param starting_angle Angle from which to draw the trail
 * @param trail_length Length of the trail in degrees
 */
void draw_dashed_arc_trail(GContext *context, int16_t center_x, int16_t center_y, uint16_t radius,
                           uint16_t starting_angle, uint16_t trail_length)
{
    uint8_t dash_length = 5;
    uint8_t gap_length = 5;

    graphics_context_set_stroke_color(context, current_arc_trail_color);
    graphics_context_set_stroke_width(context, 1);

    // Calculate angle from which to start drawing lines
    uint16_t start_trail_angle = (starting_angle - trail_length + 360) % 360;

    // Draw dashes along the trail
    for (uint16_t angle = start_trail_angle; angle < start_trail_angle + trail_length;
         angle += (dash_length + gap_length))
    {
        // Calculate single dash start and end points
        uint16_t dash_start = angle;
        uint16_t dash_end = angle + dash_length;

        // Prevent dash from exceeding max trail length
        if (dash_end > start_trail_angle + trail_length)
        {
            dash_end = start_trail_angle + trail_length;
        }

        // Get points and draw the dash
        GPoint p1 = get_point_on_circle(center_x, center_y, radius, dash_start);
        GPoint p2 = get_point_on_circle(center_x, center_y, radius, dash_end);
        graphics_draw_line(context, p1, p2);
    }
}

/**
 * Update bitmap with given resource
 * @param bitmap Pointer to the bitmap to update
 * @param resource_id The new resource ID to update the bitmap to
 */
void update_bitmap(GBitmap **bitmap, const uint32_t resource_id)
{
    if (*bitmap)
    {
        gbitmap_destroy(*bitmap);
    }
    *bitmap = gbitmap_create_with_resource(resource_id);
}

/**
 * Save the settings to persistent storage
 */
void save_settings()
{
    persist_write_data(PERSISTENT_SETTINGS_KEY, settings);
    APP_LOG(APP_LOG_LEVEL_INFO, "[C]: Settings saved to persistent storage");
}

/**
 * Load the settings from persistent storage
 */
void load_settings()
{
    if (persist_exists(PERSISTENT_SETTINGS_KEY))
    {
        persist_read_data(PERSISTENT_SETTINGS_KEY, settings);
        APP_LOG(APP_LOG_LEVEL_INFO, "[C]: Settings loaded from persistent storage. Dark theme: %s",
                settings.is_dark_theme ? "enabled" : "disabled");
    }
    else
    {
        settings.is_dark_theme = false;
        APP_LOG(APP_LOG_LEVEL_INFO, "[C]: No saved settings found, using defaults");
    }
}

/**
 * Initialize the theme system with light theme as default
 */
void init_theme()
{
    // Load settings first
    load_settings();

    // Apply the theme based on loaded settings
    if (settings.is_dark_theme)
    {
        current_background_color = BACKGROUND_COLOR_DARK;
        current_dial_glyph_color = DIAL_GLYPH_COLOR_DARK;
        current_arc_trail_color = ARC_TRAIL_COLOR_DARK;
    }
    else
    {
        current_background_color = BACKGROUND_COLOR_LIGHT;
        current_dial_glyph_color = DIAL_GLYPH_COLOR_LIGHT;
        current_arc_trail_color = ARC_TRAIL_COLOR_LIGHT;
    }
}

/**
 * Set the theme based on the given theme name
 * @param theme The theme name, e.g., "dark" or "light"
 */
void set_theme(char *theme)
{
    APP_LOG(APP_LOG_LEVEL_INFO, "Setting theme to: %s", theme);
    if (theme != NULL && strcmp(theme, "dark") == 0)
    {
        settings.is_dark_theme = true;
        current_background_color = BACKGROUND_COLOR_DARK;
        current_dial_glyph_color = DIAL_GLYPH_COLOR_DARK;
        current_arc_trail_color = ARC_TRAIL_COLOR_DARK;
    }
    else
    {
        settings.is_dark_theme = false;
        current_background_color = BACKGROUND_COLOR_LIGHT;
        current_dial_glyph_color = DIAL_GLYPH_COLOR_LIGHT;
        current_arc_trail_color = ARC_TRAIL_COLOR_LIGHT;
    }

    save_settings();
}