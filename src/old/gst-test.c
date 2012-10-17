#include <stdio.h>
#include <gst/gst.h>
#include <glib.h>

void partial_result(GstElement* object, gchararray arg0, gchararray arg1,
    gpointer user_data) {

}

void result(GstElement* object, gchararray arg0, gchararray arg1,
    gpointer user_data) {

}

int main(int argc, char** argv) {
    GMainLoop *loop;
    GstElement *pipeline, *source, *convert, *resample, *vader, *pocket,
        *fake;

    gst_init(&argc, &argv);

    loop = g_main_loop_new(NULL, FALSE);

    pipeline = gst_pipeline_new("pipeline");
    source = gst_element_factory_make("autoaudiosrc", "input");
    convert = gst_element_factory_make("audioconvert", "converter");
    resample = gst_element_factory_make("audioresample", "resample");
    vader = gst_element_factory_make("vader", "vader");
    pocket = gst_element_factory_make("pocketsphinx", "sphinx");
    fake = gst_element_factory_make("fakesink", "fake");

    if (!pipeline || !source || !convert || !resample || !vader || !pocket
        || !fake) {
        printf("Ocorreu um erro ao criar elementos!\n");
        return -1;
    }

    printf("Elementos criados com sucesso!\n");

    g_object_set(G_OBJECT(pocket), "hmm",
        "/home/aluno/workspace/VC/data/hmm/en_US/hub4wsj_sc_8k", NULL);
    g_object_set(G_OBJECT(pocket), "lm",
        "/home/aluno/workspace/VC/data/lm/en/vc.dmp", NULL);
    g_object_set(G_OBJECT(pocket), "dict",
        "/home/aluno/workspace/VC/data/lm/en/vc.dic", NULL);

    printf("1!\n");

    g_object_set(G_OBJECT(vader), "auto_threshold", 1, NULL);

    gst_bin_add_many(GST_BIN(pipeline), source, convert, resample, vader,
        pocket, fake, NULL);

    gst_element_link_many(source, convert, resample, vader, pocket, fake,
        NULL);

    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    printf("1!\n");

    g_main_loop_run(loop);

    return 0;
}
