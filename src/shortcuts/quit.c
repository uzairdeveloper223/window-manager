#include "../all.h"

HANDLE(ShortcutPressed)
{
    XClientMessageEvent *_event = &event->xclient;

    // Get shortcut name from event data.
    char shortcut[MAX_SHORTCUT_NAME];
    strncpy(shortcut, (const char*)_event->data.b, MAX_SHORTCUT_NAME - 1);
    shortcut[MAX_SHORTCUT_NAME - 1] = '\0';

    // Exit the application if shortcut name matches the quit shortcut.
    if (strcmp(shortcut, CFG_KEY_QUIT_SHORTCUT) == 0)
    {
        exit(EXIT_SUCCESS);
    }
}
