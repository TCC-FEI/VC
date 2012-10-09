#include <control/manager.h>
#include <X11/Xlib.h>
#include <libwnck/libwnck.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    vcwm_t vcwm;
    control_manager_t* manager;

    gdk_init(&argc, &argv);

    vcwm.display = XOpenDisplay(NULL);
    vcwm.screen = wnck_screen_get_default();

    wnck_screen_force_update(vcwm.screen);

    manager = control_manager_create(vcwm);
    printf("Loading gedit: %d\n", control_manager_load(manager, "gedit"));

    //sleep(5);
    control_manager_execute(manager, "gedit file new");
    sleep(5);
    control_manager_execute(manager, "gedit file close");

    control_manager_destroy(manager);
    return 0;
}
