#include "graphique.h"


// cette fonction est appeler lorsque l'utilisateur appuie sur la touche Entrée
void key_press_event(GtkWidget *window, GdkEventKey *gkey, st * s) {
  if (gkey->type == GDK_KEY_RELEASE) {
    if (gkey->keyval == 65293) {
      const gchar *str = gtk_entry_get_text((void *)s->entry);
      //fprintf(stdout, " str = %s", str);
      if (strlen(str) != 0) {
        //fprintf(stdout,"strlen message = %lu \n",strlen(str));
        append_item(NULL,(GtkWidget*) s->entry, NULL);
        if (s->arg == '1')
          envoi_sr(str);
        else
          envoi_cl(str);
        gtk_entry_set_text((GtkWidget*) s->entry, "");
      }
    }
  }
}

// cette fonction est appelèe si l'utilisateur appuie sur le boutton envoyer
void Envoyer_button(GtkWidget *window , st *s) {
  const gchar *str = gtk_entry_get_text((void *)s->entry);
  printf(" str = %s\n", str);
  if (strlen(str) != 0) {
    append_item(NULL, (GtkWidget*) s->entry, NULL);
    if ((char)s->arg == '1')
      envoi_sr(str);
    else
      envoi_cl(str);
    gtk_entry_set_text( (GtkWidget*) s->entry, "");
  }
}

// Cette fonction fait deux taches :
// 1- Affiche le message que l'utilisateur a saisie sur la fenêtre de Tchat .
// 2- Affiche le message que l'utilisateur a reçue sur la fenêtre de Tchat .
void append_item(GtkWidget *widget, gpointer entry, const char *chaine) {

  GtkListStore *store;
  GtkTreeIter iter;

  if (chaine == NULL) {
    const gchar *str = gtk_entry_get_text(entry);
    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));
    gtk_list_store_append(store, &iter);
    gchar str2[5000] = "You :   ";
    strcat(str2, str);

    gtk_list_store_set(store, &iter, LIST_ITEM, str2, -1);
  } else {
    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));
    gtk_list_store_append(store, &iter);
    gchar str2[5000] = "Friend  :  ";
    strcat(str2, chaine);
    gtk_list_store_set(store, &iter, LIST_ITEM, str2, -1);
  }
}

// Cette fonction initialise le champs d'affichage
void init_list(GtkWidget *list) {
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GtkListStore *store;
  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("List Item", renderer,
                                                    "text", LIST_ITEM, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);
  store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);
  gtk_tree_view_set_model(GTK_TREE_VIEW(list), GTK_TREE_MODEL(store));
  g_object_unref(store);
}


// Gerer le signal (SIGINT)
void sigint_handler() { mon_exit(NULL, NULL); }


