/**
 * Uno
 * Tommy MacWilliam <tmacwilliam@cs.harvard.edu>
 * Rob Bowden <rob@cs.harvard.edu>
 *
 */

#include <gtk/gtk.h>

int main(int argc, char* argv[]) {
    gtk_init(&argc, &argv);

    // create main window
    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(window), "CS50 Uno");
    gtk_widget_show(window);

    // when x is pressed, end app
    g_signal_connect_swapped(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // enter main loop
    gtk_main();
    return 0;
}
