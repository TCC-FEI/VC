#ifndef VC_SPEECH_PLUGIN_H
#define VC_SPEECH_PLUGIN_H

#include <config.h>

#include <stdio.h>
#include <glib.h>
#include <gst/gst.h>

typedef struct {
    GstElement* pipeline;
} speech_plugin_t;

#endif
