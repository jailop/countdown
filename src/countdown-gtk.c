/**
 * GTK CountDown
 *
 * A countdown timer
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include <time.h>

int start;
int limit = 30 * 60;
int remind;
int active = FALSE;
GtkWidget *button;
GtkWidget *entry;
GNotification *notify;
GtkApplication *app;

static void update_entry() {
    char text[15];
    GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
    sprintf(text, "%02d:%02d", limit / 60, limit % 60);
    gtk_entry_buffer_set_text(buffer, text, strlen(text));
}

static void restart() {
    g_application_send_notification(G_APPLICATION(app), "times-up",
            G_NOTIFICATION(notify));
    gtk_button_set_label(GTK_BUTTON(button), "Start");
    gtk_editable_set_editable(GTK_EDITABLE(entry), TRUE);
    active = FALSE;
}

static gboolean update_label(GtkWidget *entry) {
    int current = time(NULL);
    limit -= current - start;
    start = current;
    if (limit <= 0) {
        restart();
        limit = remind;
    }
    update_entry();
    if (!active)
        gtk_widget_grab_focus(GTK_WIDGET(entry));
    return active;
}

static void set_time(GtkWidget *widget, gpointer user_data) {
    char *pos = NULL;
    if (active) 
        restart();
    else {
        GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
        const char *text = gtk_entry_buffer_get_text(buffer);
        start = time(NULL);
        limit = atoi(text) * 60;
        pos = strchr(text, ':');
        if (pos)
            limit += atoi(pos + 1);
        remind = limit;
        g_timeout_add(1000, (GSourceFunc)update_label, entry);
        gtk_button_set_label(GTK_BUTTON(button), "Stop");
        gtk_editable_set_editable(GTK_EDITABLE(entry), FALSE);
        active = TRUE;
    }
    update_entry();
}

static void main_window(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *box;
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Countdown");
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_window_set_child(GTK_WINDOW(window), box);
    entry = gtk_entry_new();
    gtk_entry_set_alignment(GTK_ENTRY(entry), 1.0);
    gtk_box_append(GTK_BOX(box), entry);
    button = gtk_button_new_with_label("Start");
    gtk_box_append(GTK_BOX(box), button);
    g_signal_connect(button, "clicked", G_CALLBACK(set_time), NULL);
    g_signal_connect(entry, "activate", G_CALLBACK(set_time), NULL);
    update_entry();
    gtk_widget_set_visible(GTK_WIDGET(window), TRUE);
}

int main(int argc, char *argv[]) {
    int status;
    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(main_window), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    notify = g_notification_new("Countdown");
    g_notification_set_body(notify, "Time's up!");
    g_object_unref(app);
    return status;
}
