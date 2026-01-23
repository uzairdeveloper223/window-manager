#include "../all.h"

HANDLE(PortalButtonPress)
{
    Portal *portal = event->portal_button_press.portal;
    if (!portal->mapped) return;

    // Always set keyboard focus (external events can steal it).
    XSetInputFocus(
        DefaultDisplay,         // Display
        portal->client_window,  // Window
        RevertToPointerRoot,    // Revert To
        CurrentTime             // Time
    );

    // Raise and notify if not already on top.
    if (get_top_portal() == portal) return;
    raise_portal(portal);
    call_event_handlers((Event*)&(PortalFocusedEvent){
        .type = PortalFocused,
        .portal = portal
    });
}
