#include <X11/Xlib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <syslog.h>

#define max(a, b) (((a) > (b)) ? (a) : (b))

static char * const launcher [] = {
    "/usr/bin/rofi",
    "-show",
    "run",
    NULL
};

struct view {
    unsigned char split_ratio;
    signed char stack_count;
};

static const struct view def_view = { .split_ratio = 1, .stack_count = 0 };

