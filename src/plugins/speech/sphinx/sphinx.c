#include <vc/speech/plugin.h>

#include <gst/gst.h>
#include <stdio.h>

#include <sys/time.h>

struct speech_plugin_st {
    GstElement* pipeline;
};

void result(GstElement* object, gchararray arg0, gchararray arg1,
    gpointer data) {
    control_manager_t* control = (control_manager_t*) data;
    struct timeval start;
    struct timeval stop;
    
    printf("hipótese: %s\n", arg0);

    printf("object: [%p]\n", object);
    printf("arg0:   [%p]\n", arg0);
    printf("arg1:   [%p]\n", arg1);
    printf("data:   [%p]\n", data);
    
    gettimeofday(&start, NULL);
    
    (*(control->execute))(control, g_utf8_strdown (arg0, -1));

    gettimeofday(&stop, NULL);
    
    printf("\n\nStart: %ld.%06ld \t Stop: %ld.%06ld \t Diferenca: %ld\n\n", 
            start.tv_sec, start.tv_usec, stop.tv_sec, stop.tv_usec, stop.tv_usec - start.tv_usec);
}

gpointer create(gpointer data) {
    speech_plugin_t* self;
    GstElement *source, *convert, *resample, *vader, *pocket, *fake;
    GError* error;

    self = g_try_new0(speech_plugin_t, 1);
    if (!self) {
        printf("Falha ao alocar memória para o plugin de reconhecimento\n");
        return NULL;
    }

    printf("Iniciando Gstreamer...\n");
    if (FALSE == gst_init_check(NULL, NULL, &error)) {
        printf("Falha ao iniciar Gstreamer: %s\n", error->message);
        destroy(self);
        return NULL;
    }

    self->pipeline = gst_pipeline_new("pipeline");
    if (!self->pipeline) {
        printf("Falha ao criar a pipeline.\n");
        destroy(self);
        return NULL;
    }

    printf("Pipeline alocada em [%p].\n", self->pipeline);

    printf("Criando os elementos da pipeline...\n");

    source = gst_element_factory_make("autoaudiosrc", "input");
    convert = gst_element_factory_make("audioconvert", "converter");
    resample = gst_element_factory_make("audioresample", "resample");
    vader = gst_element_factory_make("vader", "vader");
    pocket = gst_element_factory_make("pocketsphinx", "sphinx");
    fake = gst_element_factory_make("fakesink", "fake");

    if (!source || !convert || !resample || !vader || !pocket || !fake) {
        printf("Falha ao criar elementos!\n");
        destroy(self);
        return NULL;
    }

    printf("Elementos criados com sucesso!\n");

    g_object_set(G_OBJECT(pocket), "hmm", "/tmp/data/hmm/en_US/hub4wsj_sc_8k",
        NULL);
    g_object_set(G_OBJECT(pocket), "lm", "/tmp/data/lm/en/vc.dmp", NULL);
    g_object_set(G_OBJECT(pocket), "dict", "/tmp/data/lm/en/vc.dic", NULL);

    g_object_set(G_OBJECT(vader), "auto_threshold", TRUE, NULL);

    gst_bin_add_many(GST_BIN(self->pipeline), source, convert, resample,
        vader, pocket, fake, NULL);

    gst_element_link_many(source, convert, resample, vader, pocket, fake,
        NULL);

    printf("data: [%p]\n", data);
    g_signal_connect(pocket, "result", G_CALLBACK(result), data);

    g_object_set(G_OBJECT(pocket), "configured", TRUE, NULL);

    return self;
}

void destroy(gpointer data) {
    speech_plugin_t* self = (speech_plugin_t*) data;

    if (self) {
        stop(self);
        g_free(self);
    }
}

gboolean start(gpointer data) {
    speech_plugin_t* self = (speech_plugin_t*) data;

    GstStateChangeReturn status = GST_STATE_CHANGE_FAILURE;

    if (self) {
        status = gst_element_set_state(self->pipeline, GST_STATE_PLAYING);
    }

    return status == GST_STATE_CHANGE_SUCCESS ? TRUE : FALSE;
}

gboolean stop(gpointer data) {
    speech_plugin_t* self = (speech_plugin_t*) data;
    GstStateChangeReturn status = GST_STATE_CHANGE_FAILURE;

    if (self) {
        status = gst_element_set_state(self->pipeline, GST_STATE_NULL);
    }

    return status == GST_STATE_CHANGE_SUCCESS ? TRUE : FALSE;
}
