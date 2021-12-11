#include <gtk/gtk.h>
#include "PJ_RPI.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GtkWidget *outputIO_choise;
GtkWidget *inputIO_choise;

int OutputIO;
int InputIO;
int OutputIO_state = 0;
int InputIO_state = 0;
//variables for toggeling outputs
int state = 0;

//callbalk function when closbutton is clicked
void end_program(GtkWidget *wid, gpointer ptr)
{
    gtk_main_quit();
}

//callback to set the gpio's
void SetGPIOoutput(GtkWidget *wid, gpointer ptr)
{
    const char *output = gtk_entry_get_text (GTK_ENTRY (outputIO_choise));
    OutputIO = atoi(output);
    char buffer[30];
    sprintf(buffer, "Output IO = %d", OutputIO);
    gtk_label_set_text(GTK_LABEL(ptr), buffer);
    OUT_GPIO(OutputIO);
    OutputIO_state = 1;
    GPIO_CLR = 1 << OutputIO;
    int state = 0;
}

void SetGPIOinput(GtkWidget *wid, gpointer ptr)
{
    const char *input = gtk_entry_get_text (GTK_ENTRY (inputIO_choise));
    InputIO = atoi(input);
    char buffer[30];
    sprintf(buffer, "Input IO = %d", InputIO);
    gtk_label_set_text(GTK_LABEL(ptr), buffer);
    INP_GPIO(InputIO);
    InputIO_state = 1;
}

//callback when controll input button is clicked
void controlOutput1(GtkWidget *wid, gpointer ptr)
{
    if(OutputIO_state == 1){
        if (state == 0)
        {
            GPIO_SET = (1 << OutputIO) ^ GPIO_SET;
            state = 1;
            gtk_label_set_text(GTK_LABEL(ptr), "GPIO STATE = 1       ");
        }
        else
        {
            state = 0; 
            GPIO_CLR = 1 << OutputIO;
            gtk_label_set_text(GTK_LABEL(ptr), "GPIO STATE = 0         ");                       
        }
    }
    else{
        gtk_label_set_text(GTK_LABEL(ptr), "please pick output first");
    }
}
void ShowInputs(GtkWidget *wid, gpointer ptr)
{
    int valueInp = 0;

   if(InputIO_state == 1 ){
        if (GPIO_READ(InputIO))
        { // !=0 <-> bit is 1 <- port is HIGH=3.3V
            valueInp = 1;
        }
        else
        { // port is LOW=0V
            valueInp = 0;
        }
        char buffer[30];
            int count = 1;
            sprintf(buffer, "Input = %d             ", valueInp);
            gtk_label_set_text(GTK_LABEL(ptr), buffer);
   }else{
        gtk_label_set_text(GTK_LABEL(ptr), "please pick input first");
   }
}

void main(int argc, char *argv[])
{
    //try to map gpio
    if (map_peripheral(&gpio) == -1)
    {
        printf("Failed to map the physical GPIO registers into the virtual memory space.\n");
    }

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
    GtkWidget *lblInp = gtk_label_new("                       ");

    //input IO label
    GtkWidget *lblInpIo = gtk_label_new("Input IO =  ");
     //input IO label
    GtkWidget *lblOutpIo = gtk_label_new("Output IO =  ");

    //add button to control IO
    //als we dan willen instellen dat we kunnen kiezen welke IO we aansturen dan moeten we dat lbl meegeven en gebruiken in de functie
    GtkWidget *lblOutpError = gtk_label_new("                       ");
    GtkWidget *btnOUT1 = gtk_button_new_with_label("Toggle Output");
    g_signal_connect(btnOUT1, "clicked", G_CALLBACK(controlOutput1), lblOutpError);

    GtkWidget *btnShowInp = gtk_button_new_with_label("Show Inputs");
    g_signal_connect(btnShowInp, "clicked", G_CALLBACK(ShowInputs), lblInp);

    //for textboxes for in and output choise
    GtkObject *adj = gtk_adjustment_new (0, 0, 30, 1, 0, 0);
    outputIO_choise = gtk_spin_button_new (GTK_ADJUSTMENT (adj), 0, 0);

    GtkObject *adj2 = gtk_adjustment_new (0, 0, 30, 1, 0, 0);
    inputIO_choise = gtk_spin_button_new (GTK_ADJUSTMENT (adj2), 0, 0);

    GtkWidget *btnSetOutp = gtk_button_new_with_label("Set Output number");
    g_signal_connect(btnSetOutp, "clicked", G_CALLBACK(SetGPIOoutput), lblOutpIo);

    GtkWidget *btnSetInp = gtk_button_new_with_label("Set Input number");
    g_signal_connect(btnSetInp, "clicked", G_CALLBACK(SetGPIOinput), lblInpIo);

    //window layout
    //create box = assigns the same amount of space to every widget it holds/ to align
    //horizonal box to align output choise box and button horizonaly
    GtkWidget *boxOutpCh = gtk_hbox_new(FALSE, 20);
    gtk_box_pack_start(GTK_BOX(boxOutpCh), outputIO_choise, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(boxOutpCh), btnSetOutp, TRUE, TRUE, 5);

    //horizonal box to align input choise box and button horizonaly
    GtkWidget *boxInpCh = gtk_hbox_new(FALSE, 20);
    gtk_box_pack_start(GTK_BOX(boxInpCh), inputIO_choise, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(boxInpCh), btnSetInp, TRUE, TRUE, 5);

     GtkWidget *boxIolbl = gtk_hbox_new(FALSE, 20);
    gtk_box_pack_start(GTK_BOX(boxIolbl), lblInpIo, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(boxIolbl), lblOutpIo, TRUE, TRUE, 5);


    //horizonal box to align output button horizonaly
    GtkWidget *boxOutp = gtk_hbox_new(FALSE, 20);
    gtk_box_pack_start(GTK_BOX(boxOutp), lblOutpError, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(boxOutp), btnOUT1, TRUE, TRUE, 5);

    //horizonal box to align input lbl adn input button labels horizonaly
    GtkWidget *boxInp = gtk_hbox_new(FALSE, 20);
    gtk_box_pack_start(GTK_BOX(boxInp), lblInp, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(boxInp), btnShowInp, TRUE, TRUE, 15);

    //horizonal box to align close window button horizonaly
    GtkWidget *boxCl = gtk_hbox_new(FALSE, 20);
    gtk_box_pack_start(GTK_BOX(boxCl), btn, TRUE, TRUE, 20);

    //vertical box to align all horizontal boxes verticaly
    GtkWidget *boxTotal = gtk_vbox_new(FALSE, 5);
    gtk_box_pack_start(GTK_BOX(boxTotal), boxOutpCh, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(boxTotal), boxInpCh, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(boxTotal), boxIolbl, TRUE, TRUE, 5);

    gtk_box_pack_start(GTK_BOX(boxTotal), boxOutp, TRUE, TRUE, 15);
    gtk_box_pack_start(GTK_BOX(boxTotal), boxInp, TRUE, TRUE, 15);

    gtk_box_pack_start(GTK_BOX(boxTotal), boxCl, TRUE, TRUE, 30);

    //add the vertical total box to the window and show them
    gtk_container_add(GTK_CONTAINER(win), boxTotal);
    gtk_widget_show_all(win);
    gtk_main();
}