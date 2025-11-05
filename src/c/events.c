#include "events.h"
#include "base.h"
#include "main.h"
#include "message_keys.auto.h"

#include <pebble-events/pebble-events.h>

/**
 * Callback for when a message is received in the inbox
 * @param iter Dictionary iterator containing the message
 * @param context User-defined context (unused)
 */
static void inbox_received_callback(DictionaryIterator *iter, void *context)
{
    if (!iter)
        return;

    Tuple *type_tuple = dict_find(iter, MESSAGE_KEY_type);
    if (!type_tuple)
    {
        APP_LOG(APP_LOG_LEVEL_ERROR, "[C]: Message received without type");
        return;
    }

    if (strcmp(type_tuple->value->cstring, "update_theme") == 0)
    {
        APP_LOG(APP_LOG_LEVEL_INFO, "[C]: Handling update theme message");
        Tuple *state_tuple = dict_find(iter, MESSAGE_KEY_state);
        if (!state_tuple)
        {
            APP_LOG(APP_LOG_LEVEL_ERROR, "[C]: Update theme message missing state");
            return;
        }

        if (strcmp(state_tuple->value->cstring, "success") == 0)
        {
            Tuple *theme_tuple = dict_find(iter, MESSAGE_KEY_theme);
            if (theme_tuple)
            {
                char *theme = theme_tuple->value->cstring;
                set_theme(theme);
                redraw_all_assets();
            }
        }
        else
        {
            APP_LOG(APP_LOG_LEVEL_ERROR, "[C]: Update theme failed with state: %s", state_tuple->value->cstring);
        }
    }
    else
    {
        APP_LOG(APP_LOG_LEVEL_WARNING, "[C]: Ignoring message type: %s", type_tuple->value->cstring);
    }
}

/**
 * Initialize the event system
 */
void events_init(void)
{
    events_app_message_request_inbox_size(60);
    events_app_message_register_inbox_received(inbox_received_callback, NULL);
    events_app_message_open();
}

/**
 * Clean up the events system
 */
void events_deinit(void)
{
    // Nothing to clean up
}