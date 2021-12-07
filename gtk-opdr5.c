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

void ShowInputs(GtkWidget *wid, gpointer ptr)
{
    int valueInp1 = 0;
    int valueInp2 = 0;
    if (GPIO_READ(22))
    { // !=0 <-> bit is 1 <- port is HIGH=3.3V
        valueInp1 = 1;
    }
    else
    { // port is LOW=0V
        valueInp1 = 0;
    }
    if (GPIO_READ(26))
    { // !=0 <-> bit is 1 <- port is HIGH=3.3V
        valueInp2 = 1;
    }
    else
    { // port is LOW=0V
        valueInp2 = 0;
    }
    
    char buffer[30];
    int count = 1;
    sprintf(buffer, "Input 22 = %d, Input 26 = %d", valueInp1, valueInp2);
    gtk_label_set_text(GTK_LABEL(ptr), buffer);
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
    //set GPIO on Input
    INP_GPIO(22);
    INP_GPIO(26);

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
    GtkWidget *lblInp = gtk_label_new("Input 22 =  , Input 26 =  ");

    //add button to control IO
    //als we dan willen instellen dat we kunnen kiezen welke IO we aansturen dan moeten we dat lbl meegeven en gebruiken in de functie
    GtkWidget *btnOUT1 = gtk_button_new_with_label("Toggle GPIO 17");
    g_signal_connect(btnOUT1, "clicked", G_CALLBACK(controlOutput1), NULL);

    GtkWidget *btnOUT2 = gtk_button_new_with_label("Toggle GPIO 27");
    g_signal_connect(btnOUT2, "clicked", G_CALLBACK(controlOutput2), NULL);

    GtkWidget *btnShowInp = gtk_button_new_with_label("Show Inputs");
    g_signal_connect(btnShowInp, "clicked", G_CALLBACK(ShowInputs), lblInp);

    //create box = assigns the same amount of space to every widget it holds/ to align
    //horizonal box to align output buttons horizonaly
    GtkWidget *boxOutp = gtk_hbox_new(FALSE, 20);
    gtk_box_pack_start(GTK_BOX(boxOutp), btnOUT1, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(boxOutp), btnOUT2, TRUE, TRUE, 5);

    //horizonal box to align input labels horizonaly
    GtkWidget *boxInp = gtk_hbox_new(FALSE, 20);
    gtk_box_pack_start(GTK_BOX(boxInp), lblInp, TRUE, TRUE, 10);

    //horizonal box to align close button horizonaly
    GtkWidget *boxCl = gtk_hbox_new(FALSE, 20);
    gtk_box_pack_start(GTK_BOX(boxCl), btn, TRUE, TRUE, 20);

    //vertical box to align als horizontal boxes verticaly
    GtkWidget *boxTotal = gtk_vbox_new(FALSE, 5);
    gtk_box_pack_start(GTK_BOX(boxTotal), boxOutp, TRUE, TRUE, 30);
    gtk_box_pack_start(GTK_BOX(boxTotal), boxInp, TRUE, TRUE, 15);
    gtk_box_pack_start(GTK_BOX(boxTotal), btnShowInp, TRUE, TRUE, 15);
    gtk_box_pack_start(GTK_BOX(boxTotal), boxCl, TRUE, TRUE, 30);

    //add the vertical total box to the window and show them
    gtk_container_add(GTK_CONTAINER(win), boxTotal);
    gtk_widget_show_all(win);
    gtk_main();
}