#include "clserv.h"
#include "graphique.h"
#include <gtk/gtk.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int sock = 0, n = 0;
socklen_t fromlen = 0;
struct sockaddr_in server_sr = {0};
struct sockaddr_in server_cl = {0};
struct sockaddr_in from_cl = {0};
struct sockaddr_in from_sr = {0};
socklen_t length = 0;
struct hostent *hp = 0;
char arg;
pthread_t threads[2];
st *s;




// Cette fonction est appelè en cas ou l'utilisateur quite le tchat avec (Ctrl +c) ou en appuyant sur la boutton 
// exit en haut à droite , pour attendre les
// threads et pour libèrer la mèmoire .
void mon_exit(GtkWidget *widget, gpointer callback_data) {
  int rc = pthread_kill(threads[0], SIGUSR1);
  if (rc < 0)
    perror("pthread_kill 0 failed");
  rc = pthread_kill(threads[1], SIGUSR1);
  if (rc < 0)
    perror("pthread_kill 1 failed");
  rc = pthread_join(threads[0], NULL);
  if (rc < 0)
    perror("pthread_join 0 failed");
  rc = pthread_join(threads[1], NULL);
  if (rc < 0)
    perror("pthread_join 1 failed");
  close(sock);

  free(s);
  gtk_main_quit();
}




int main(int argc, char *argv[]) {
  signal(SIGINT, sigint_handler);
  arg = argv[1][0];

  // partie rèseau
  if (argc < 3) {
    fprintf(stderr, "ERROR ARGS : Example : \n \\
                         serveur  :  ./tchat <1> <port> \n \\ 
                         client   :  ./tchat <2> <localhost> <port> \n");
    exit(0);
  }
  if (arg == '1') {
    // partie serveur
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
      error("Socket erreur ");
    }

    length = sizeof(server_sr);
    bzero(&server_sr, length);
    server_sr.sin_family = AF_INET;
    server_sr.sin_addr.s_addr = INADDR_ANY;
    server_sr.sin_port = htons(atoi(argv[2]));
    if (bind(sock, (struct sockaddr *)&server_sr, length) < 0) {
      error("binding");
    }
    fromlen = sizeof(struct sockaddr_in);

  } else if (arg == '2') {
    // partie cient
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
      error("socket");
    }
    server_cl.sin_family = AF_INET;
    hp = gethostbyname(argv[2]);
    if (hp == 0) {
      error("Unknown Host");
    }
    bcopy((char *)hp->h_addr, (char *)&server_cl.sin_addr, hp->h_length);
    server_cl.sin_port = htons(atoi(argv[3]));

    length = sizeof(struct sockaddr_in);
  } else {

    fprintf(stderr, "ERROR, ARGS \n");
    exit(0);
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////
  // partie graphique
  GtkWidget *window;
  GtkWidget *sw;
  GtkWidget *Envoyer;
  GtkWidget *entry;
  GtkWidget *fixed;
  gtk_init(&argc, &argv);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), " Mon Tchat ");
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);
  gtk_widget_set_size_request(window, 900, 900);
  fixed = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(window), fixed);
  sw = gtk_scrolled_window_new(NULL, NULL);
  list = gtk_tree_view_new();
  gtk_container_add(GTK_CONTAINER(sw), list);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw), GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(sw),
                                      GTK_SHADOW_ETCHED_IN);

  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);
  // Creation du boutton Envoyer
  Envoyer = gtk_button_new_with_label("Envoyer");
  // Creation du champs de saisie
  entry = gtk_entry_new();
  // Positionnement des widgets
  gtk_fixed_put(GTK_FIXED(fixed), entry, 110, 720);
  gtk_widget_set_size_request(entry, 620, 40);
  gtk_fixed_put(GTK_FIXED(fixed), Envoyer, 735, 720);
  gtk_widget_set_size_request(Envoyer, 100, 40);
  gtk_fixed_put(GTK_FIXED(fixed), sw, 150, 90);
  gtk_widget_set_size_request(sw, 600, 600);
  init_list(list);
  s = (st *)malloc(sizeof(s));
  s->entry = entry;
  s->arg = arg;
  // Gerer l'èvenement Click sur le boutton Envoyer
  g_signal_connect(G_OBJECT(Envoyer), "clicked", G_CALLBACK(Envoyer_button), s);
  // Gerer l'èvenement de quitter le tchat
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(mon_exit), NULL);
  // Evenement clavier
  g_signal_connect(GTK_OBJECT(window), "key-release-event",
                   G_CALLBACK(key_press_event), s);
  gtk_widget_show_all(window);
  /////////////////////////////////////////////////////////////////////////////:
  // Partie exècution des threads
  if (argv[1][0] == '1') {
    // server
    pthread_create(threads, NULL, premiere_connection_sr, NULL);
    pthread_create(threads + 1, NULL, reception_sr, NULL);
  } else if (argv[1][0] == '2') {
    // client
    pthread_create(threads, NULL, premiere_connection_cl, NULL);
    pthread_create(threads + 1, NULL, reception_cl, NULL);
  }

  gtk_main();

  return 0;
}