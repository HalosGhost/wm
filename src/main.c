#include "main.h"

signed
main (void) {

    Display * dpy = 0;
    XWindowAttributes attr;
    XButtonEvent start;

    if ( !(dpy = XOpenDisplay(0)) ) { return EXIT_FAILURE; }

    XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("F1")), Mod1Mask,
            DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync);
    XGrabButton(dpy, 1, Mod1Mask, DefaultRootWindow(dpy), True,
            ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);
    XGrabButton(dpy, 3, Mod1Mask, DefaultRootWindow(dpy), True,
            ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);

    start.subwindow = None;
    do {
        XEvent ev;
        XNextEvent(dpy, &ev);

        switch ( ev.type ) {
            case KeyPress:
                if ( ev.xkey.subwindow != None ) {
                    XRaiseWindow(dpy, ev.xkey.subwindow);
                } break;

            case ButtonPress:
                if ( ev.xbutton.subwindow != None ) {
                    XGetWindowAttributes(dpy, ev.xbutton.subwindow, &attr);
                    start = ev.xbutton;
                } break;

            case MotionNotify: {
                int xdiff = ev.xbutton.x_root - start.x_root;
                int ydiff = ev.xbutton.y_root - start.y_root;
                XMoveResizeWindow(dpy, start.subwindow,
                    attr.x + (start.button == 1 ? xdiff : 0),
                    attr.y + (start.button == 1 ? ydiff : 0),
                    (unsigned )max(1, attr.width + (start.button == 3 ? xdiff : 0)),
                    (unsigned )max(1, attr.height + (start.button == 3 ? ydiff : 0)));
            } break;

            case ButtonRelease:
                start.subwindow = None;
                break;
        }
    } while (true);
}

