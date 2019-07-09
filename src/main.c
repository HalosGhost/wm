#include "main.h"

signed
main (void) {

    Display * dpy = 0;
    if ( !(dpy = XOpenDisplay(0)) ) { return EXIT_FAILURE; }

    Window root = DefaultRootWindow(dpy);

    XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("d")), Mod1Mask,
            root, true, GrabModeAsync, GrabModeAsync);

    do {
        static XEvent ev;
        XNextEvent(dpy, &ev);

        switch ( ev.type ) {
            case KeyPress:
                if ( fork() != -1 && setsid() != -1 ) {
                    execv(*launcher, launcher);
                } break;
        }
    } while (true);
}

