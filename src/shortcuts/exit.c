#include "../all.h"

HANDLE(ShortcutPressed)
{
    XClientMessageEvent *_event = &event->xclient;

    // Get shortcut name from event data.
    char shortcut_name[MAX_SHORTCUT_NAME];
    strncpy(shortcut_name, (const char*)_event->data.b, MAX_SHORTCUT_NAME - 1);
    shortcut_name[MAX_SHORTCUT_NAME - 1] = '\0';

    // Ensure we're handling the exit shortcut.
    if (strcmp(shortcut_name, CFG_KEY_EXIT_SHORTCUT) != 0) return;
    
    exit(EXIT_SUCCESS);
}
