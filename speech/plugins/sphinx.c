#include <stdio.h>
#include <glib.h>
#include <gst/gst.h>
#include <plugin.h>

void partial_result(GstElement*, gchararray, gchararray, gpointer);
void result(GstElement*, gchararray, gchararray, gpointer);

speech_plugin_t* speech_plugin_create(int* argc, char*** argv) {
    
    speech_plugin_t* self;
    GstElement *pipeline, 
               *source, 
               *convert, 
               *resample, 
               *vader, 
               *pocket,
               *fake;

    printf("Iniciando Gstreamer...\n");
    gst_init(argc, argv);
    
    self = (speech_plugin_t*) malloc(sizeof(speech_plugin_t));

    self->pipeline = gst_pipeline_new("pipeline");
    
    if(!self->pipeline){
        printf("Nao foi possivel criar a pipeline.\n");
        speech_plugin_destroy(self);
        return NULL;
    }
    
    printf("Pipeline criada no endereco [%p].\n", self->pipeline);
    
    printf("Criando os elementos da pipeline...\n");
    
    source = gst_element_factory_make("autoaudiosrc", "input");
    convert = gst_element_factory_make("audioconvert", "converter");
    resample = gst_element_factory_make("audioresample", "resample");
    vader = gst_element_factory_make("vader", "vader");
    pocket = gst_element_factory_make("pocketsphinx", "sphinx");
    fake = gst_element_factory_make("fakesink", "fake");
    
    printf("Criou elementos!\n");

    if (!source || !convert || !resample || !vader || !pocket || !fake) {
        printf("Ocorreu um erro ao criar elementos!\n");
        speech_plugin_destroy(self);
        return NULL;
    }

    printf("Elementos criados com sucesso!\n");
    
     g_object_set(G_OBJECT(pocket), "hmm",
        "/home/aluno/workspace/VC/data/hmm/en_US/hub4wsj_sc_8k", NULL);
    g_object_set(G_OBJECT(pocket), "lm",
        "/home/aluno/workspace/Temp/new/7569.dmp", NULL);
    g_object_set(G_OBJECT(pocket), "dict",
        "/home/aluno/workspace/Temp/new/7569.dic", NULL);

    g_object_set(G_OBJECT(vader), "auto_threshold", TRUE, NULL);

    gst_bin_add_many(GST_BIN(self->pipeline), source, convert, resample, vader,
        pocket, fake, NULL);

    gst_element_link_many(source, convert, resample, vader, pocket, fake,
        NULL);

    g_signal_connect (pocket, "partial-result", G_CALLBACK (partial_result), NULL);
    g_signal_connect (pocket, "result", G_CALLBACK (result), NULL);

    g_object_set(G_OBJECT(pocket), "configured", TRUE, NULL);
    
    return self;
}

void speech_plugin_destroy(speech_plugin_t* plugin ) {
    
}

gboolean speech_plugin_start(speech_plugin_t* plugin ) {
    gst_element_set_state(plugin->pipeline, GST_STATE_PLAYING);
    return FALSE;
}

gboolean speech_plugin_stop(speech_plugin_t* plugin ) {
    gst_element_set_state(plugin->pipeline, GST_STATE_NULL);
    return FALSE;
}

void partial_result(GstElement* object, gchararray arg0, gchararray arg1,
    gpointer user_data) {
    printf("Partial Result!\n");
    printf("%s\n\n", arg0);
}

void result(GstElement* object, gchararray arg0, gchararray arg1,
    gpointer user_data) {
    printf("Result!\n");
    printf("%s\n\n", arg0);
}

