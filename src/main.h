#include <X11/Xlib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#define max(a, b) (((a) > (b)) ? (a) : (b))

static char * const launcher [] = {
    "/usr/bin/rofi",
    "-show",
    "run",
    NULL
};

