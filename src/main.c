#include <vc/vc.h>
#include <vc/app.h>

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    app_t app;
    control_manager_args_t ctrl_args;

    GThread* cmd_th;
    GError* cmd_th_err;
    int return_code;

    vc_trace("Inicicalizando GDK...\n");
    gdk_init(&argc, &argv);

    app.main_loop = g_main_loop_new(NULL, FALSE);
    vc_trace("Criando GMainLoop:          [%p]\n", app.main_loop);

    ctrl_args.vcwm.display = XOpenDisplay(NULL);
    vc_trace("Conectando-se ao Display:   [%p]\n", ctrl_args.vcwm.display);

    ctrl_args.vcwm.screen = wnck_screen_get_default();
    wnck_screen_force_update(ctrl_args.vcwm.screen);
    vc_trace("Obtendo WnckScreen default: [%p]\n", ctrl_args.vcwm.screen);

    ctrl_args.plugin_dir = PREFIX "/lib/plugins/";
    vc_trace("Diretório de plugins em '%s'\n", ctrl_args.plugin_dir);

    if (!ctrl_args.vcwm.display || !ctrl_args.vcwm.screen) {
        vc_trace("Display ou WnckScren inválidos. Não posso continuar...\n");
        return EXIT_FAILURE;
    }

    app.control = control_manager_create(&ctrl_args);
    app.speech = speech_manager_create(app.control);
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
