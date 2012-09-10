#include <stdio.h>
#include <gst/gst.h>
#include <glib.h>

void run_loop(GMainLoop **loop){
    g_main_loop_run(*loop);
    return;
}

void quit_loop(GMainLoop **loop){
    g_main_loop_quit(*loop);
    return;
}

int main (int argc, char** argv){

    GMainLoop *loop;
    GstElement *pipeline, *source, *convert, *resample, *level,
               *vader, *pocket, *file;
    GstCaps *caps;

    gst_init (&argc, &argv);

    caps = caps = gst_caps_from_string ("audio/x-raw-int, "
                                        "width = (int) 16, "
                                        "depth = (int) 16, "
                                        "signed = (boolean) true, "
                                        "endianness = (int) BYTE_ORDER, "
                                        "channels = (int) 1, "
                                        "rate = (int) 16000");

    loop = g_main_loop_new(NULL, FALSE);

    pipeline = gst_pipeline_new("pipeline");
    source   = gst_element_factory_make("autoaudiosrc", "input");
    convert  = gst_element_factory_make("audioconvert", "converter");
    resample = gst_element_factory_make("audioresample", "resample");
    vader    = gst_element_factory_make("vader", "vader");
    level    = gst_element_factory_make("level", "level");
    pocket   = gst_element_factory_make("pocketsphinx", "sphinx");
    file     = gst_element_factory_make("filesink", "file-sink");

    if(!pipeline || !source || !convert || !resample || !vader ||
       !level || !pocket || !file){
        printf("Ocorreu um erro ao criar elementos!\n");
        return -1;
    }

    g_object_set (G_OBJECT (file), "location", "goforward.txt", NULL);
    g_object_set (G_OBJECT (pocket), "hmm", "/usr/local/share/pocketsphinx/model/hmm/en_US/hub4wsj_sc_8k", NULL);
    g_object_set (G_OBJECT (pocket), "lm", "/usr/local/share/pocketsphinx/model/lm/en/vc.dmp", NULL);
    g_object_set (G_OBJECT (pocket), "dict", "/usr/local/share/pocketsphinx/model/lm/en/vc.dic", NULL);
    g_object_set (G_OBJECT (vader), "auto_threshold", "true", NULL);

    gst_bin_add_many(GST_BIN(pipeline), source, convert, resample, vader,
                     pocket, file, NULL);

//    gst_element_link(source, convert);
//    gst_element_link_filtered (convert, level, caps);
//    gst_element_link(level, file);

    gst_element_link_many(source, convert, resample, vader, pocket, file, NULL);

    gst_element_set_state(pipeline, GST_STATE_PLAYING);
    run_loop(&loop);

    return 0;
}
