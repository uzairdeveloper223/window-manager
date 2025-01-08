#include "../all.h"

int x_get_window_name(Display *display, Window window, char *out_name, size_t name_size)
{
    // List of properties to check for the window name.
    const int property_count = 2;
    Atom properties[property_count];
    properties[0] = XInternAtom(display, "_NET_WM_NAME", False);
    properties[1] = XInternAtom(display, "WM_NAME", False);

    // Loop over the properties, and stores the first one that is available.
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

bool x_window_supports_protocol(Display *display, Window window, Atom protocol)
{
    Atom *protocols;
    int count;
    if (XGetWMProtocols(display, window, &protocols, &count))
    {
        for (int i = 0; i < count; i++)
        {
            if (protocols[i] == protocol)
            {
                XFree(protocols);
                return True;
            }
        }
        XFree(protocols);
    }
    return False;
}

bool x_window_exists(Display *display, Window window)
{
    return XGetWindowAttributes(display, window, &(XWindowAttributes){0}) != 0;
}

int x_key_name_to_symbol(const char *name, int *out_key)
{
    // Create a copy of the provided key name string.
    char *name_copy = strdup(name);

    // Make the key name string lowercase.
    for (int i = 0; i < (int)strlen(name_copy); i++)
    {
        name_copy[i] = tolower(name_copy[i]);
    }

    // Convert the key name to a key symbol.
    if (strcmp(name_copy, "super") == 0)
    {
        *out_key = XK_Super_L;
    }
    else if (strcmp(name_copy, "control") == 0)
    {
        *out_key = XK_Control_L;
    }
    else if (strcmp(name_copy, "ctrl") == 0)
    {
        *out_key = XK_Control_L;
    }
    else if (strcmp(name_copy, "shift") == 0)
    {
        *out_key = XK_Shift_L;
    }
    else if (strcmp(name_copy, "alt") == 0)
    {
        *out_key = XK_Alt_L;
    }
    else if (strcmp(name_copy, "enter") == 0)
    {
        *out_key = XK_Return;
    }
    else if (strcmp(name_copy, "esc") == 0)
    {
        *out_key = XK_Escape;
    }
    else
    {
        // The `XStringToKeysym()` function expects the first character to be
        // uppercase, with the exception of single letters (e.g. 'a', 'b', 'c').
        if (strlen(name_copy) > 1)
        {
            name_copy[0] = toupper(name_copy[0]);
        }
        *out_key = XStringToKeysym(name_copy);
    }

    // Free the duplicate string.
    free(name_copy);

    return (out_key != NoSymbol) ? 0 : -1;
}

int x_key_names_to_symbols(char *names, const char delimiter, int *out_keys, int keys_size)
{
    int status = 0;

    // Create a copy of the provided key names string.
    char *names_copy = strdup(names);

    // Make the key names string lowercase.
    for (int i = 0; i < (int)strlen(names_copy); i++)
    {
        names_copy[i] = tolower(names_copy[i]);
    }
    
    // Split the key names string by the delimiter and iterate over each token.
    char *token = strtok(names_copy, &delimiter);
    for (int i = 0; i < keys_size; i++)
    {
        // If we ran out of tokens, but the loop isn't completed yet, fill the 
        // remaining key symbol slots with NoSymbol.
        if (token == NULL)
        {
            out_keys[i] = NoSymbol;
            continue;
        }

        // Convert the token (key name) to a key symbol.
        int key = 0;
        x_key_name_to_symbol(token, &key);
        if (key == NoSymbol) status = -1;
        out_keys[i] = key;

        // Prepare for the next iteration.
        token = strtok(NULL, &delimiter);
    }

    // Free the duplicate string.
    free(names_copy);

    return status;
}
