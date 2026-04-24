#include "glib.h"
#include <cstdio>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <math.h>

const int RGB = 255; 
const int ACCURACY = 1;
const int WIDTH = 800;
const int SUPER_WIDTH = WIDTH * ACCURACY;
const int HEIGHT = 600;
const int SUPER_HEIGHT = HEIGHT * ACCURACY;


const int nMax = 255;
const int Rmax = 10;
const double SCALE_X = 4.0;
const double SCALE_Y = 3.5;
const double BEAUTY_COEF = -0.6;
const double DX = 1.0/(double)(SUPER_WIDTH);
const double DY = 1.0/(double)(SUPER_HEIGHT);
const double DELTA = 0.1;

struct maldebrot_params{
    GtkWidget *drawing_area;
    GtkWidget *window;
    int* color_maldebrot;
    int PALETTE[RGB + 1];
    double zoom;
    double center_x;
    double center_y;
    double time;
}; 

void generate_mandelbrot(maldebrot_params* params, int* color_maldebrot);