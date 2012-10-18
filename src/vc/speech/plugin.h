#ifndef SPHINX_H
#define SPHINX_H

#include <stdio.h>
#include <glib.h>
#include <gst/gst.h>

typedef struct {
    GstElement* pipeline;
} speech_plugin_t;

speech_plugin_t* speech_plugin_create(int*, char***);
void speech_plugin_destroy(speech_plugin_t*);
gboolean speech_plugin_start(speech_plugin_t*);
gboolean speech_plugin_stop(speech_plugin_t*);

#endif
