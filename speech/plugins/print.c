#include <stdio.h>
#include <glib.h>
#include <gst/gst.h>
#include <speech/plugin.h>

void partial_result(GstElement*, gchararray, gchararray, gpointer);
void result(GstElement*, gchararray, gchararray, gpointer);
void recognize_command(gchararray);

speech_plugin_t* speech_plugin_create(int* argc, char*** argv) {
    printf("create\n");
}

void speech_plugin_destroy(speech_plugin_t* plugein ) {
    printf("destroy\n");
}

gboolean speech_plugin_start(speech_plugin_t* plugin) {
    printf("start\n");
}

gboolean speech_plugin_stop(speech_plugin_t* plugin ) {
    printf("stop\n");
}

void recognize_command(gchararray command) {
    printf("recognize\n");
}

void partial_result(GstElement* object, gchararray arg0, gchararray arg1,
    printf("partial_result\n");
}

void result(GstElement* object, gchararray arg0, gchararray arg1,
    printf("result\n");
}

