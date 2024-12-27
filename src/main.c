#include "all.h"

int main()
{
    Display *display = XOpenDisplay(NULL);
    if (!display) {
        LOG_ERROR("Failed to open X11 display.");
        exit(EXIT_FAILURE);
    }

    XSetErrorHandler(x_error_handler);

    initialize_event_loop(display, DefaultRootWindow(display));
    
    return 0;
}
