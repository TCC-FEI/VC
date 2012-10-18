#include <stdio.h>
#include <glib.h>
#include <gst/gst.h>
#include <speech/manager.h>
#include <speech/handler.h>
#include <speech/plugin.h>

int main(int argc, char** argv){
    
    GMainLoop* loop;
    speech_manager_t* manager;
    
    loop = g_main_loop_new(NULL, FALSE);
    
    manager = speech_manager_create(&argc, &argv);
    
    if(!manager->handler) {
        printf("Nao foi possivel criar plugin\n");
        speech_manager_destroy(manager);
        return -1;
    }
    
    if(!speech_manager_load(manager, "sphinx")){
        printf("Nao foi possivel carregar plugin\n");
        speech_manager_destroy(manager);
        return -1;
    }
    
    if(!(*(manager->handler->start))(manager->handler->instance)){
        printf("Nao foi possivel modificar o status da pipeline para PLAYING\n");
        speech_manager_destroy(manager);
        return -1;
    }
    
    printf("Pipeline em status PLAYING\n");
    
    g_main_loop_run(loop);
        
    return 0;
};
