#ifndef APP_H
#define APP_H

#include <glib.h>
#include <control/manager.h>

#define MAX_BUFF_SIZE   256


typedef struct {
    GMainLoop*  main_loop;
    control_manager_t* manager;
} app_t;


#endif
