#pragma once

#include "@pebble-libraries/debug-tick-timer-service/debug-tick-timer-service.h"
#include "@pebble-libraries/pbl-stdlib/pbl-stdlib.h"
#include "ball.h"
#include "base.h"
#include "doggo.h"
#include "events.h"

extern Window *main_window;
extern Layer *background;
extern Layer *dial;

extern GFont kagi_font;

void redraw_all_assets();