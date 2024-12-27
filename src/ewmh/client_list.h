#pragma once
#include "../all.h"

#ifdef STATIC

/**
 * Updates the `_NET_CLIENT_LIST` property on the root window, setting it to the
 * current list of client windows. EWMH specification requires every window 
 * manager to advertise a list of client windows this way so that other 
 * applications can query it.
 * 
 * @param display The X11 display.
 * @param root_window The root window.
 */
static void publish_ewmh_client_list(Display *display, Window root_window);

/**
 * Adds a window to our internal list of EWMH client windows.
 * 
 * @param window The window to add to the list.
 * 
 * @note `publish_ewmh_client_list()` must be called after this function to
 * update the `_NET_CLIENT_LIST` property on the root window.
 */
static void add_to_ewmh_client_list(Window window);

/**
 * Removes a window from our internal list of EWMH client windows.
 * 
 * @param window The window to add to the list.
 * 
 * @note `publish_ewmh_client_list()` must be called after this function to
 * update the `_NET_CLIENT_LIST` property on the root window.
 */
static void remove_from_ewmh_client_list(Window window);

/**
 * Checks if a window is in our internal list of EWMH client windows.
 * 
 * @param window The window to check.
 * @return True (1) if the window is in the list, False (0) otherwise.
 */
static Bool is_in_ewmh_client_list(Window window);

#endif
