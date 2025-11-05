#include "@pebble-libraries/debug-tick-timer-service/debug-tick-timer-service.h"
#include "@pebble-libraries/pbl-stdlib/pbl-stdlib.h"
#include "ball.h"
#include "base.h"
#include "doggo.h"

static Window *main_window;
static Layer *background;
static Layer *dial;

static GFont kagi_font;
