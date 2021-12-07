#include <gtk/gtk.h>
#include "PJ_RPI.h"

//variables for toggeling outputs
int state1 = 0;
int state2 = 0;

//callbalk function when closbutton is clicked
void end_program(GtkWidget *wid, gpointer ptr)
{
    gtk_main_quit();
}

//callback when controll input button is clicked
void controlOutput1(GtkWidget *wid, gpointer ptr)
{
    /*char buffer[30];
 int count = 1;
 sprintf (buffer, "Button pressed %d times", count);
 gtk_label_set_text (GTK_LABEL (ptr), buffer);*/
    if (state1 == 0)
    {
        GPIO_SET = (1 << 17) ^ GPIO_SET;
        state1 = 1;
    }
    else
    {
        state1 = 0;
        GPIO_CLR = 1 << 17;
    }
}

void controlOutput2(GtkWidget *wid, gpointer ptr)
{
    if (state2 == 0)
    {
        GPIO_SET = (1 << 27) ^ GPIO_SET;
        state2 = 1;
    }
    else
    {
        GPIO_CLR = 1 << 27;
        state2 = 0;
    }
}

void main(int argc, char *argv[])
{
    //try to map gpio
    if (map_peripheral(&gpio) == -1)
    {
        printf("Failed to map the physical GPIO registers into the virtual memory space.\n");
    }

    //set GPIO on Output
    OUT_GPIO(17);
    OUT_GPIO(27);
    //set them initial on zero
    GPIO_CLR = 1 << 17;
    GPIO_CLR = 1 << 17;

    //init gtk
    gtk_init(&argc, &argv);
    //create window
    GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    //create signal for button to end program
    GtkWidget *btn = gtk_button_new_with_label("Close window");
    g_signal_connect(btn, "clicked", G_CALLBACK(end_program), NULL);
    //create signal for when cross to close window is clicked
    g_signal_connect(win, "delete_event", G_CALLBACK(end_program), NULL);

    //create labels for inputs
    GtkWidget *lblInp1 = gtk_label_new("Input 1:");
    GtkWidget *lblInp2 = gtk_label_new("Input 2:");

    //add button to control IO
    //als we dan willen instellen dat we kunnen kiezen welke IO we aansturen dan moeten we dat lbl meegeven en gebruiken in de functie
    GtkWidget *btnOUT1 = gtk_button_new_with_label("Toggle GPIO 17");
    g_signal_connect(btnOUT1, "clicked", G_CALLBACK(controlOutput1), lblInp1);

    GtkWidget *btnOUT2 = gtk_button_new_with_label("Toggle GPIO 27");
    g_signal_connect(btnOUT2, "clicked", G_CALLBACK(controlOutput2), NULL);

    //create box = assigns the same amount of space to every widget it holds/ to align
     //horizonal box to align output buttons horizonaly
    GtkWidget *boxOutp = gtk_hbox_new(FALSE, 20);
    gtk_box_pack_start(GTK_BOX(boxOutp), btnOUT1, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(boxOutp), btnOUT2, TRUE, TRUE, 5);

    //horizonal box to align input labels horizonaly
    GtkWidget *boxInp = gtk_hbox_new(FALSE, 20);
    gtk_box_pack_start(GTK_BOX(boxInp), lblInp1, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(boxInp), lblInp2, TRUE, TRUE, 10);

    //horizonal box to align close button horizonaly
    GtkWidget *boxCl = gtk_hbox_new(FALSE, 20);
    gtk_box_pack_start(GTK_BOX(boxCl), btn, TRUE, TRUE, 20);

    //vertical box to align als horizontal boxes verticaly
    GtkWidget *boxTotal = gtk_vbox_new(FALSE, 5);
    gtk_box_pack_start(GTK_BOX(boxTotal), boxOutp, TRUE, TRUE, 30);
    gtk_box_pack_start(GTK_BOX(boxTotal), boxInp, TRUE, TRUE, 30);
    gtk_box_pack_start(GTK_BOX(boxTotal), boxCl, TRUE, TRUE, 30);

    //add the vertical total box to the window and show them
    gtk_container_add(GTK_CONTAINER(win), boxTotal);
    gtk_widget_show_all(win);
    gtk_main();
}