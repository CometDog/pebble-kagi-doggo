#include "doggo.h"
#include "ball.h"
#include "@pebble-libraries/debug-tick-timer-service/debug-tick-timer-service.h"
#include "@pebble-libraries/pbl-stdlib/pbl-stdlib.h"

#ifdef PBL_BW
#define BACKGROUND_COLOR GColorWhite
#else
#define BACKGROUND_COLOR GColorFromHEX(0x9DEBFE)
#endif

static Window *main_window;
static Layer *background;
static Layer *dial;

static GFont kagi_font;
