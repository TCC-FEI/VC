#include <vc/vc.h>
#include <vc/app.h>

#include <glib.h>
#include <stdio.h>

int main(int argc, char** argv) {
    app_t app;
    vcwm_t vcwm;

    GThread* cmd_th;
    GError* cmd_th_err;
    int return_code;

    vc_trace("Inicicalizando GDK...\n");
    gdk_init(&argc, &argv);

    app.main_loop = g_main_loop_new(NULL, FALSE);
    vc_trace("Criando GMainLoop:          [%p]\n", app.main_loop);

    vcwm.display = XOpenDisplay(NULL);
    vc_trace("Conectando-se ao Display:   [%p]\n", vcwm.display);
    vcwm.screen = wnck_screen_get_default();
    vc_trace("Obtendo WnckScreen default: [%p]\n", vcwm.screen);
    wnck_screen_force_update(vcwm.screen);

    app.control = control_manager_create(vcwm);
    app.speech  =  speech_manager_create();
    if (app.control && app.speech) {
        cmd_th = g_thread_try_new("console", console, &app, &cmd_th_err);
        if (cmd_th) {
            g_main_loop_run(app.main_loop);
        } else {
            printf("g_thread error: %s\n", cmd_th_err->message);
        }

        control_manager_destroy(app.control);
        speech_manager_destroy(app.speech);
    } else {
        vc_trace("Falha ao inicializar componentes\n");
    }

    return return_code;
}
