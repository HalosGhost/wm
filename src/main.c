#include "main.h"

signed
main (int argc, char * argv []) {

    Display * dpy = 0;
    if ( !(dpy = XOpenDisplay(0)) || argc < 1 ) { return EXIT_FAILURE; }

    openlog(argv[0], 0, LOG_USER);

    Window root = DefaultRootWindow(dpy);

    XWindowAttributes attr;
    XGetWindowAttributes(dpy, root, &attr);

    XSelectInput(dpy, root, SubstructureNotifyMask);

    XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("d")), Mod1Mask,
            root, true, GrabModeAsync, GrabModeAsync);

    Window * children = 0;
    unsigned i = 0;

    do {
        static XEvent ev;
        XNextEvent(dpy, &ev);

        static Window rt, par;
        XQueryTree(dpy, root, &rt, &par, &children, &i);

        switch ( ev.type ) {
            case CreateNotify:
                XMoveResizeWindow(dpy, ev.xcreatewindow.window, 0, 0, (unsigned )attr.width, (unsigned )attr.height);
                break;

            case KeyPress:
                if ( fork() != -1 && setsid() != -1 ) {
                    execv(*launcher, launcher);
                } break;
        }
    } while (true);
}

