#ifndef TEST_H
#define TEST_H

#include <gtk/gtk.h>

enum { LIST_ITEM = 0, N_COLUMNS };
GtkWidget *list;

struct structure{
    GtkWidget *entry;
    char arg;
};

typedef struct structure st;
extern void key_press_event(GtkWidget *window, GdkEventKey *gkey,
                            st * s);
void append_item(GtkWidget *widget, gpointer entry, const char *chaine);
extern void init_list(GtkWidget *list);
extern void sigint_handler();
extern void Envoyer_button(GtkWidget *window, st * s);


#endif