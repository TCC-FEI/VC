#include <glib.h>
#include <libwnck/libwnck.h>

#include <cspi/spi.h>
#include <libspi/keymasks.h>

#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>
#include <gdk/gdkx.h>

/*typedef struct {
 GObject parent;

 GQueue *event_queue;
 GSList *actions;
 Accessible *root;

 void *window_listener;
 void *showing_listener;
 void *keys_listener;

 gboolean is_keypressed;

 guint idle_id;
 } ControlSpiListener;

 static void control_spi_listener_window_listener_cb(
 const AccessibleEvent *event, void *user_data) {
 ControlSpiListener *listener = CONTROL_SPI_LISTENER(user_data);
 AccessibleEvent_ref(event);
 g_queue_push_tail(listener->event_queue, (gpointer) event);

 if (!listener->idle_id)
 listener->idle_id = g_idle_add(control_spi_listener_process_event,
 listener);
 }
 */
int main(int argc, char **argv) {
    //ControlSpiListener listener;

    WnckScreen *screen;
    WnckWindow *active_window;
    GList *window_l;

    KeyCode keycode;

    /*listener.event_queue = g_queue_new();
     listener.window_listener = SPI_createAccessibleEventListener(
     control_spi_listener_window_listener_cb, listener);
     listener.showing_listener = SPI_createAccessibleEventListener(
     control_spi_listener_showing_listener_cb, listener);
     listener.keys_listener = SPI_createAccessibleEventListener(
     control_spi_listener_keys_listener_cb, listener);

     SPI_registerGlobalEventListener(listener.window_listener,
     "window:activate");
     SPI_registerGlobalEventListener(listener.showing_listener,
     "object:state-changed:showing");
     SPI_registerGlobalEventListener(listener.keys_listener,
     "keyboard:modifiers");*/

    gtk_init(&argc, &argv);
    SPI_init();

    screen = wnck_screen_get_default();

    wnck_screen_force_update(screen);

    active_window = wnck_screen_get_active_window(screen);

    for (window_l = wnck_screen_get_windows(screen); window_l != NULL;
        window_l = window_l->next) {
        WnckWindow *window = WNCK_WINDOW(window_l->data);
        g_print("%s%s\n", wnck_window_get_name(window),
            window == active_window ? " (active)" : "");
    }

    keycode = XKeysymToKeycode(gdk_x11_get_default_xdisplay(),
        (KeySym) GDK_KEY_Control_L);
    SPI_generateKeyboardEvent(keycode, NULL, SPI_KEY_PRESS);
    SPI_generateKeyboardEvent(GDK_KEY_Shift_L, NULL, SPI_KEY_SYM);
    SPI_generateKeyboardEvent(GDK_KEY_Tab, NULL, SPI_KEY_SYM);
    SPI_generateKeyboardEvent(keycode, NULL, SPI_KEY_RELEASE);

    SPI_exit();

    return 0;
}
