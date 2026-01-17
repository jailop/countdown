/**
 * GTK CountDown
 *
 * A countdown timer. Built wit Gtk4
 *
 * (c) 2025-2026 - Jaime Lopez - https://github.com/jailop
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include <time.h>
#include <glib.h>

// Labels are defined as icons using unicode characters,
// as well as text.
#define ICO_START "media-playback-start"
#define ICO_RESET "view-refresh"
#define ICO_STOP "media-playback-stop"
#define LBL_START "Start"
#define LBL_RESET "Reset"
#define LBL_STOP "Stop"

// App parameters
int start;
int last = 30 * 60;
int limit = 30 * 60;
int remind;
int active = FALSE;

// App global widgets
GtkWidget *btn_start;
GtkWidget *btn_reset;
GtkWidget *entry;
GNotification *notify;
GtkApplication *app;
int prev_min = -1;  // for minute change

static void beep_sequence() {
    for(int i = 0; i < 5; i++) {
        gdk_display_beep(gdk_display_get_default());
        g_usleep(200000);
    }
}

static void set_entry_bg(const char *class_name) {
    gtk_widget_remove_css_class(entry, "green-bg");
    gtk_widget_remove_css_class(entry, "red-bg");
    gtk_widget_remove_css_class(entry, "grey-bg");
    if (class_name)
        gtk_widget_add_css_class(entry, class_name);
}

/**
 * Formats the entry as minutes and seconds
 */
static void update_entry() {
    char text[15];
    GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
    sprintf(text, "%02d:%02d", limit / 60, limit % 60);
    gtk_entry_buffer_set_text(buffer, text, strlen(text));
}

static void _restart() {
    gtk_button_set_icon_name(GTK_BUTTON(btn_start), ICO_START);
    gtk_widget_set_tooltip_text(btn_start, LBL_START);
    gtk_editable_set_editable(GTK_EDITABLE(entry), TRUE);
    active = FALSE;
}



static void reset(GtkWidget *widget, gpointer user_data) {
    char last_str[15];
    _restart();    
    limit = last;
    // sprintf(last_str, "%d", last);
    // GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
    // gtk_entry_buffer_set_text(buffer, last_str, 15);
    set_entry_bg(NULL);
    prev_min = -1;
    update_entry();
}

static gboolean remove_bg(gpointer user_data) {
    set_entry_bg(NULL);
    return FALSE;
}

static gboolean update_label(GtkWidget *entry) {
    int current = time(NULL);
    limit -= current - start;
    start = current;
    if (limit <= 0) {
        set_entry_bg("red-bg");
        g_application_send_notification(G_APPLICATION(app), "times-up", 
                G_NOTIFICATION(notify));
        beep_sequence();
        _restart();
        limit = remind;
    } else {
        int curr_min = limit / 60;
        if (curr_min != prev_min && prev_min != -1) {
            set_entry_bg("grey-bg");
            g_timeout_add(3000, (GSourceFunc) remove_bg, NULL);
        }
        prev_min = curr_min;
    }
    update_entry();
    if (!active)
        gtk_widget_grab_focus(GTK_WIDGET(entry));
    return active;
}

static void set_time(GtkWidget *widget, gpointer user_data) {
    char *pos = NULL;
    if (active) {
        set_entry_bg("green-bg");
        prev_min = limit / 60;
        _restart();
    } else {
        GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
        const char *text = gtk_entry_buffer_get_text(buffer);
        start = time(NULL);
        limit = atoi(text) * 60;
        last = limit;
        pos = strchr(text, ':');
        if (pos)
            limit += atoi(pos + 1);
        remind = limit;
        g_timeout_add(1000, (GSourceFunc) update_label, entry);
        gtk_button_set_icon_name(GTK_BUTTON(btn_start), ICO_STOP);
        gtk_widget_set_tooltip_text(btn_start, LBL_STOP);
        gtk_editable_set_editable(GTK_EDITABLE(entry), FALSE);
        active = TRUE;
    }
    update_entry();
}

static void main_window(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Countdown");
    gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_widget_set_opacity(window, 0.6);
    
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_window_set_child(GTK_WINDOW(window), hbox);
    
    entry = gtk_entry_new();
    gtk_editable_set_width_chars(GTK_EDITABLE(entry), 5);
    gtk_entry_set_has_frame(GTK_ENTRY(entry), FALSE);
    gtk_entry_set_max_length(GTK_ENTRY(entry), 8);
    gtk_entry_set_alignment(GTK_ENTRY(entry), 1.0);
    gtk_box_append(GTK_BOX(hbox), entry);
    
    btn_start = gtk_button_new_from_icon_name(ICO_START);
    btn_reset = gtk_button_new_from_icon_name(ICO_RESET);
    gtk_widget_set_tooltip_text(btn_start, LBL_START);
    gtk_widget_set_tooltip_text(btn_reset, LBL_RESET);

    gtk_box_append(GTK_BOX(hbox), btn_start);
    gtk_box_append(GTK_BOX(hbox), btn_reset);

    g_signal_connect(btn_start, "clicked", G_CALLBACK(set_time), NULL);
    g_signal_connect(btn_reset, "clicked", G_CALLBACK(reset), NULL);
    g_signal_connect(entry, "activate", G_CALLBACK(set_time), NULL);

    GtkCssProvider* css = gtk_css_provider_new();
    gtk_css_provider_load_from_string(css,
            ".green-bg { background-color: lightgreen; }"
            ".red-bg { background-color: pink; }"
            ".grey-bg { background-color: lightgrey; }");
    gtk_style_context_add_provider_for_display(gdk_display_get_default(),
            GTK_STYLE_PROVIDER(css), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(css);

    update_entry();
    gtk_widget_set_visible(GTK_WIDGET(window), TRUE);
}

int main(int argc, char *argv[]) {
    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(main_window), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    notify = g_notification_new("Countdown");
    g_notification_set_body(notify, "Time's up!");
    g_object_unref(app);
    return status;
}
