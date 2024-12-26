#include "../all.h"

int x_get_window_name(Display *display, Window window, char *out_name, size_t name_size)
{
    Atom net_wm_name = XInternAtom(display, "_NET_WM_NAME", False);
    Atom wm_name = XInternAtom(display, "WM_NAME", False);

    const int property_count = 2;
    Atom properties[property_count];
    properties[0] = net_wm_name;
    properties[1] = wm_name;

    unsigned char *name = NULL;
    for(int i = 0; i < property_count; i++)
    {
        int status = XGetWindowProperty(
            display,                // Display
            window,                 // Window
            properties[i],          // Property
            0, (~0L),               // Offset, length
            False,                  // Delete
            AnyPropertyType,        // Type
            &(Atom){0},             // Actual type (unused)
            &(int){0},              // Actual format (unused)
            &(unsigned long){0},    // N items (unused)
            &(unsigned long){0},    // Bytes after (unused)
            &name                   // Data
        );
        if (status == Success && name != NULL)
        {
            strncpy(out_name, (const char *)name, name_size);
            out_name[name_size - 1] = '\0';  // Ensure null termination.

            XFree(name);
            break;
        }
    }

    return (name != NULL) ? 0 : -1;
}

int x_error_handler(Display *display, XErrorEvent *error) {
    if (
        // Ignore BadWindow, BadDrawable and BadPixmap errors since they commonly
        // occur, even under normal operation. All other X errors are logged as
        // they typically indicate real issues.
        error->error_code != BadWindow &&
        error->error_code != BadDrawable &&
        error->error_code != BadPixmap
    )
    {
        char error_text[1024];
        XGetErrorText(display, error->error_code, error_text, sizeof(error_text));
        
        LOG_ERROR(
            "%s (request: %d, resource: 0x%lx)", 
            error_text, error->request_code, error->resourceid
        );
    }
    return 0;
}
