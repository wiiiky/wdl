#include <gtk/gtk.h>
#include "wlbter.h"

#define SPEED_K 1000
#define SPEED_B_STR  "B/s"
#define SPEED_K_STR "kB/s"
#define SPEED_M_STR "MB/s"
#define SPEED_G_STR "GB/s"
#define SPEED_T_STR "TB/s"


int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 480, 300);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    g_signal_connect(G_OBJECT(window), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), box);

    tr_formatter_speed_init(SPEED_K, SPEED_K_STR, SPEED_M_STR, SPEED_G_STR,
                            SPEED_T_STR);

    tr_session *session;
    tr_variant settings;
    const char *configDir = tr_getDefaultConfigDir("wdl");
    tr_variantInitDict(&settings, 0);
    tr_sessionGetDefaultSettings(&settings);
    session = tr_sessionInit("gtk", configDir, true, &settings);
    tr_variantFree(&settings);

    WlBter *bter = wl_bter_new_from_file(session,
                                         "/home/wiky/test.torrent");
    wl_bter_start(bter);
    gtk_box_pack_start(GTK_BOX(box), GTK_WIDGET(bter), FALSE, FALSE, 0);

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
