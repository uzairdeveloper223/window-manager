#include "../all.h"

static int shortcut_input[MAX_SHORTCUT_KEYS];

static void add_key_to_shortcut_input(int key)
{
    // Ensure the shortcut_input array is not full.
    if(shortcut_input[MAX_SHORTCUT_KEYS - 1] != 0) return;

    // Add the key to the shortcut_input array.
    for (int i = 0; i < MAX_SHORTCUT_KEYS; i++)
    {
        if (shortcut_input[i] == 0)
        {
            shortcut_input[i] = key;
            break;
        }
    }
}

static void remove_key_from_shortcut_input(int key)
{
    for (int i = 0; i < MAX_SHORTCUT_KEYS; i++)
    {
        if (shortcut_input[i] == key)
        {
            shortcut_input[i] = 0;
            return;
        }
    }
}

static bool is_key_in_shortcut_input(int key)
{
    for (int i = 0; i < MAX_SHORTCUT_KEYS; i++)
    {
        if (shortcut_input[i] == key)
        {
            return true;
        }
    }
    return false;
}

static void check_shortcut_input(Display *display)
{
    // Check if the current shortcut input matches any registered shortcuts.
    char shortcut_name[MAX_SHORTCUT_NAME];
    int result = find_shortcut(shortcut_input, MAX_SHORTCUT_KEYS, shortcut_name, MAX_SHORTCUT_NAME);
    if (result != 0)
    {
        return;
    }

    // Invoke the ShortcutPressed event.
    XEvent event;
    event.xclient.type = ClientMessage;
    event.xclient.display = display;
    event.xclient.window = DefaultRootWindow(display);
    event.xclient.message_type = XInternAtom(display, "_WM_SHORTCUT_PRESSED", False);
    event.xclient.format = 8;
    strncpy(event.xclient.data.b, shortcut_name, sizeof(event.xclient.data.b));
    invoke_event_handlers(display, DefaultRootWindow(display), ShortcutPressed, &event);
}

HANDLE(GlobalKeyPress)
{
    XKeyPressedEvent *_event = &event->xkey;
    KeySym key = XkbKeycodeToKeysym(display, _event->keycode, 0, 0);

    if(is_key_in_shortcut_input(key)) return;
    add_key_to_shortcut_input(key);
    check_shortcut_input(display);
}

HANDLE(GlobalKeyRelease)
{
    XKeyReleasedEvent *_event = &event->xkey;
    KeySym key = XkbKeycodeToKeysym(display, _event->keycode, 0, 0);

    if(!is_key_in_shortcut_input(key)) return;
    remove_key_from_shortcut_input(key);
}
