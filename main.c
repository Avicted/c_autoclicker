// =============================================
// Program: Autoclicker for UNIX systems
// Author: Victor Anderssén
// Version: 1.0
// Description: Autoclicker for UNIX
// Compile: build.sh
// Date: 3.3.2024
// =============================================

// Standard library headers
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

// X11 specific headers
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>

// UNIX specific headers
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

// Defines
#define internal static // Internal for the translation unit

// Types

// Structure to hold data for the autoclick thread
typedef struct
{
    Display *display;
} AutoclickThreadData;

// Variables
internal bool volatile autoclick = false;
internal bool volatile autoclick_thread_running = false; // Track if the autoclick thread is running
internal const size_t clicks_hertz = 368;

internal void
pause_but_do_not_sleep(int delay)
{
    clock_t start_time = clock();

    // looping till required time is not achieved
    while (clock() < start_time + delay)
    {
        asm("nop"); // No Operation
    }
}

internal void *
autoclick_thread(void *arg)
{
    AutoclickThreadData *data = (AutoclickThreadData *)arg;
    Display *display = data->display;

    while (autoclick)
    {
        XTestFakeButtonEvent(display, 1, True, CurrentTime);
        XFlush(display);

        // Pause for the specified amount of time, but do not sleep
        pause_but_do_not_sleep(1000000 / clicks_hertz);

        XTestFakeButtonEvent(display, 1, False, CurrentTime);
        XFlush(display);
    }

    autoclick_thread_running = false; // Update the flag when the thread exits

    return (NULL);
}

internal void
SigIntHandler(int Signal)
{
    printf("\tCaught SIGINT, exiting peacefully!\n");

    exit(0);
}

int main(void)
{
    Display *display;
    Window root;
    XEvent event;

    // Handle signals for CTRL+C
    signal(SIGINT, SigIntHandler);

    // Open the display
    display = XOpenDisplay(NULL);
    if (display == NULL)
    {
        fprintf(stderr, "\tError: Could not open display\n");
        return (1);
    }

    // Get the root window
    root = XDefaultRootWindow(display);

    // Grab the scroll lock key
    XGrabKey(display, XKeysymToKeycode(display, XK_Scroll_Lock), AnyModifier, root,
             True, GrabModeAsync, GrabModeAsync);

    // Initialize autoclick thread data with calloc
    AutoclickThreadData autoclick_thread_data = {0};

    autoclick_thread_data.display = display;

    pthread_t thread_id;
    int rc;

    // Print the clicks_hertz
    printf("Clicks per second: %ld\n", clicks_hertz);

    // Event loop
    while (1)
    {
        XNextEvent(display, &event);

        if (event.type == KeyPress)
        {
            // Check if the scroll lock key is pressed
            if (event.xkey.keycode == XKeysymToKeycode(display, XK_Scroll_Lock))
            {
                // Toggle autoclick on / off
                autoclick = !autoclick;

                printf("\tAutoclick is %s\n", autoclick ? "enabled" : "disabled");

                if (autoclick && !autoclick_thread_running)
                {
                    // Create the autoclick thread only if it's not already running
                    rc = pthread_create(&thread_id, NULL, autoclick_thread, (void *)&autoclick_thread_data);

                    if (rc)
                    {
                        fprintf(stderr, "\tError: Failed to create thread\n");
                        autoclick = false; // Disable autoclicking if thread creation fails
                    }
                    else
                    {
                        autoclick_thread_running = true; // Update the flag
                    }
                }
                else if (!autoclick)
                {
                    // Join the autoclick thread if autoclick is being disabled
                    pthread_join(thread_id, NULL);
                }
            }
        }
    }

    // Cleanup
    XUngrabKey(display, XKeysymToKeycode(display, XK_Scroll_Lock), AnyModifier, root);
    XCloseDisplay(display);

    free(&autoclick_thread_data); // Free the autoclick thread data (not really necessary, but good practice

    return (0);
}
