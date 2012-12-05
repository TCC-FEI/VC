#ifndef VC_CONTROL_TYPES_H
#define VC_CONTROL_TYPES_H

#include <X11/Xlib.h>
#include <libwnck/libwnck.h>

typedef struct {
    Display* display;
    WnckScreen* screen;
} vcwm_t;

#endif
