#include <glib.h>
#include <stdio.h>

#include <app.h>
#include <console/console.h>


int main(int argc, char** argv) {
    app_t app;

    GThread* cmd_th;
    GError* cmd_th_err;
    int return_code;

    app.main_loop = g_main_loop_new(NULL, FALSE);

    if (cmd_th = g_thread_try_new("console", console, &app, &cmd_th_err)) {
        g_main_loop_run( app.main_loop );
    }
    else {
        printf("g_thread error: %s\n", cmd_th_err->message );
    }

    return return_code;
}
