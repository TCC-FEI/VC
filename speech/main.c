#include <stdio.h>
#include <glib.h>
#include <gst/gst.h>
#include <plugin.h>

int main(int argc, char** argv){
    speech_plugin_t* bin;
    
    bin = speech_plugin_create(&argc, &argv);
    
    return 0;
};
