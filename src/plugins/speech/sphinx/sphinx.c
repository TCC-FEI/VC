#include <vc/speech/plugin.h>

#include <stdlib.h>
#include <stdio.h>

speech_plugin_t* create() {
    printf("speech_plugin_create\n");
}

void destroy(speech_plugin_t* self) {
    printf("speech_plugin_destroy\n");
}

gboolean start(speech_plugin_t* self) {
    printf("speech_plugin_start\n");
}

gboolean stop(speech_plugin_t* self) {
    printf("speech_plugin_stop\n");
}
