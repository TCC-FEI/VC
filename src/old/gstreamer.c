#include <stdio.h>
#include <gst/gst.h>
#include <glib.h>

int main (int argc, char** argv){

    GMainLoop *loop;
    GstElement *pipeline, *source, *convert, *level, *file;
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
    level    = gst_element_factory_make("level", "level");
    file     = gst_element_factory_make("filesink", "file-sink");

    if(!pipeline || !source || !convert || !level || !file){
        printf("Ocorreu um erro ao criar elementos!\n");
        return -1;
    }

    printf("Elementos criados com sucesso!\n");

    g_object_set (G_OBJECT (file), "location",
                                   "/home/aluno/workspace/PocketSphinx/goforward.raw",
                                   NULL);

    gst_bin_add_many(GST_BIN(pipeline), source, convert, level, file, NULL);

    gst_element_link(source, convert);

    gst_element_link_filtered (convert, level, caps);

    gst_element_link(level, file);

    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    g_main_loop_run(loop);

    g_main_loop_quit(loop);

    return 0;
}
