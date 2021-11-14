#ifndef TEST_H
#define TEST_H

#include <gtk/gtk.h>

extern void key_press_event(GtkWidget *window, GdkEventKey *gkey,
                            GtkWidget *label);
extern void append_item(GtkWidget *widget, GtkWidget *entry,
                        const char *chaine);
extern void init_list(GtkWidget *list);
extern void mon_exit(GtkWidget *widget, gpointer callback_data);
extern void sigint_handler();
extern void Envoyer_button(GtkWidget *window, GtkWidget *entry);


#endif