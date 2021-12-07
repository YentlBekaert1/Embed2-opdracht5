#include <gtk/gtk.h>

//callbalk function when closbutton is clicked
void end_program (GtkWidget *wid, gpointer ptr)
{
 gtk_main_quit ();
}

//callback when controll input button is clicked
void controlOutput1 (GtkWidget *wid, gpointer ptr)
{
 char buffer[30];
 int count = 1;
 sprintf (buffer, "Button pressed %d times", count);
 gtk_label_set_text (GTK_LABEL (ptr), buffer);
}

void controlOutput2 (GtkWidget *wid, gpointer ptr)
{
 char buffer[30];
 int count = 1;
 sprintf (buffer, "Button pressed %d times", count);
 gtk_label_set_text (GTK_LABEL (ptr), buffer);
}

void main (int argc, char *argv[])
{
 //init gtk
 gtk_init (&argc, &argv);
 //create window
 GtkWidget *win = gtk_window_new (GTK_WINDOW_TOPLEVEL);

 //create signal for button to end program
 GtkWidget *btn = gtk_button_new_with_label ("Close window");
 g_signal_connect (btn, "clicked", G_CALLBACK (end_program),NULL);
 //create signal for when cross to close window is clicked
 g_signal_connect (win, "delete_event", G_CALLBACK (end_program),NULL);

 //create label
 GtkWidget *lbl = gtk_label_new ("My label");

//add button to control IO
//als we dan willen instellen dat we kunnen kiezen welke IO we aansturen dan moeten we dat lbl meegeven en gebruiken in de functie
 GtkWidget *btnOUT1 = gtk_button_new_with_label ("Toggle GPIO 1");
 g_signal_connect (btnOUT1, "clicked", G_CALLBACK (controlOutput1), lbl);

 GtkWidget *btnOUT2 = gtk_button_new_with_label ("Toggle GPIO 2");
 g_signal_connect (btnOUT2, "clicked", G_CALLBACK (controlOutput2), NULL);

 //create vbox = assigns the same amount of space to every widget it holds
 GtkWidget *box = gtk_vbox_new (FALSE, 5);
 gtk_box_pack_start (GTK_BOX (box), btnOUT1, TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (box), btnOUT2, TRUE, TRUE, 0);

 gtk_box_pack_start (GTK_BOX (box), lbl, TRUE, TRUE, 0);
 gtk_box_pack_start (GTK_BOX (box), btn, TRUE, TRUE, 0);

 //add the vbox to the window and show them
 gtk_container_add (GTK_CONTAINER (win), box);
 gtk_widget_show_all (win);
 gtk_main ();
}