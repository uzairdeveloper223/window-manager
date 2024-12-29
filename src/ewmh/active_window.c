#include "all.h"

/**
 * This code is responsible for setting the `_NET_ACTIVE_WINDOW` property on the
 * root window. EWMH specification requires window managers to advertise this
 * property so other applications can determine the currently active window.
 * 
 * @note https://specifications.freedesktop.org/wm-spec/1.5/ar01s03.html#id-1.4.10
 */

static Window current_active_window = 0;

static void set_ewmh_active_window(Display *display, Window client_window)
{
    // Update our internal state.
    current_active_window = client_window;

    // Set the `_NET_ACTIVE_WINDOW` property.
    Atom net_active_window = XInternAtom(display, "_NET_ACTIVE_WINDOW", False);
    XChangeProperty(
        display,                            // Display
        DefaultRootWindow(display),         // Window
        net_active_window,                  // Atom / Property
        XA_WINDOW,                          // Type
        32,                                 // Format (32-bit)
        PropModeReplace,                    // Mode
        (unsigned char *)&client_window,    // Data
        1                                   // Element Count
    );
}

HANDLE(Initialize)
{
    set_ewmh_active_window(display, 0);
}

HANDLE(PortalFocused)
{
    XClientMessageEvent *_event = &event->xclient;
    Portal *portal = (Portal *)_event->data.l[0];

    set_ewmh_active_window(portal->display, portal->client_window);
}

HANDLE(PortalDestroyed)
{
    XClientMessageEvent *_event = &event->xclient;
    Window client_window = (Window)_event->data.l[0];

    if(current_active_window == client_window)
    {
        set_ewmh_active_window(display, 0);
    }
}
