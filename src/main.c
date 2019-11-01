#include "main.h"

signed
main (int argc, char * argv []) {

    Display * dpy = 0;
    signed status = EXIT_SUCCESS;
    if ( !(dpy = XOpenDisplay(0)) || argc < 1 ) { return EXIT_FAILURE; }

    // basename
    const char * progname = argv[0];
    while ( progname && *progname ) { ++ progname; }
    while ( progname != argv[0] && *(progname - 1) != '/' ) { -- progname; }

    openlog(progname, 0, LOG_USER);
    syslog(LOG_INFO, "running…\n");

    Window root = DefaultRootWindow(dpy);

    XWindowAttributes attr;
    XGetWindowAttributes(dpy, root, &attr);

    XSelectInput(dpy, root, SubstructureNotifyMask);

    XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("d")), Mod1Mask,
            root, true, GrabModeAsync, GrabModeAsync);

    Window * children = 0;
    unsigned i = 0;

    signal(SIGINT, signal_handler);
    signal(SIGHUP, signal_handler);
    signal(SIGQUIT, signal_handler);
    signal(SIGTERM, signal_handler);

    do {
        if ( !run_state ) { goto cleanup; }

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

            case ClientMessage:
                syslog(LOG_INFO, "Intercepted client messsage\n");
                break;
        }
    } while ( true );

    cleanup:
        syslog(LOG_INFO, "stopping\n");
        closelog();
        XCloseDisplay(dpy);

    return status;
}

void
signal_handler (signed signum) {

    run_state = !signum;
}

