#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

int main(int argc, char *argv[])
{
     gtk_init(&argc, &argv);

         GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
      gtk_window_set_default_size(GTK_WINDOW(window), 700, 580);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

        GtkWidget* fixed_container = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), fixed_container);

    GtkWidget *button = gtk_button_new_with_label("Get");
     gtk_fixed_put(GTK_FIXED(fixed_container), button, 655, 10);

     GtkWidget *entry1 = gtk_entry_new();
      gtk_fixed_put(GTK_FIXED(fixed_container), entry1, 10, 10);
      gtk_widget_set_size_request(entry1, 645, 15);
    gtk_entry_set_text(GTK_ENTRY(entry1),"Search");

         GtkWidget *text1 = gtk_text_view_new();
      gtk_fixed_put(GTK_FIXED(fixed_container), text1, 10, 50);
          //gtk_container_add(GTK_CONTAINER(scrolledwindow), text1);
      gtk_widget_set_size_request(text1, 675, 520);
          gtk_text_view_set_wrap_mode(text1,GTK_WRAP_WORD_CHAR);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
