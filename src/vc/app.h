#ifndef APP_H
#define APP_H

#include <glib.h>
#include <vc/control/manager.h>
#include <vc/speech/manager.h>
#include <vc/console/console.h>

#define MAX_BUFF_SIZE   256


typedef struct {
    GMainLoop*  main_loop;
    control_manager_t* control;
    speech_manager_t* speech;
} app_t;


#endif
