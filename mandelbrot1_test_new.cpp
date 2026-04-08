#include "glib.h"
#include <cmath>
#include <cstdio>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <math.h>

/*
g++ -O3 mandelbrot1.cpp -o mandelbrot1 `pkg-config --cflags --libs gtk+-3.0` -lm
./mandelbrot1

TESTS
g++ -O0 -D MEASURE_MANDELBROT mandelbrot1_test_new.cpp -o mandelbrot1 `pkg-config --cflags --libs gtk+-3.0` -lm
g++ -O3 -D MEASURE_MANDELBROT mandelbrot1_test_new.cpp -o mandelbrot1 `pkg-config --cflags --libs gtk+-3.0` -lm

taskset -c 4 ./mandelbrot1
*/

const int ACCURACY = 1;
const int WIDTH = 800;
const int SUPER_WIDTH = WIDTH * ACCURACY;
const int HEIGHT = 600;
const int SUPER_HEIGHT = HEIGHT * ACCURACY;

const int COUNTER = 1000;
const int HEAT_TESTS = 20;
const int nMax = 255;
const int Rmax = 10;
const double SCALE = 3.5;
const double BEAUTY_COEF = -0.6;
const int RGB = 255;
const double DX = 1.0/(double)(SUPER_WIDTH);
const double DY = 1.0/(double)(SUPER_HEIGHT);
const double DELTA = 0.1;


struct maldebrot_params{
    GtkWidget *drawing_area;
    int PALETTE[RGB + 1];
    double zoom;
    double center_x;
    double center_y;
};

GtkWidget *create_window(maldebrot_params* params);

GtkWidget *create_drawing_area(maldebrot_params* params);

static void generate_mandelbrot(maldebrot_params* params, int* color_maldebrot);

static void draw_maldebrot(GtkWidget *widget, cairo_t *cr, gpointer data);

static void generate_palette(maldebrot_params* params);

static void zoom_graphic(GtkWidget *widget, GdkEventKey *event, gpointer data);

static void test_mandelbrot(maldebrot_params* params);

int main(int argc, char*argv[]){
    maldebrot_params* params = (maldebrot_params*)calloc(sizeof(maldebrot_params), 1);
    params->zoom = 1.0;
    generate_palette(params);

#ifdef MEASURE_MANDELBROT
    test_mandelbrot(params);
#endif //MEASURE_MANDELBROT

    gtk_init(&argc, &argv);

    GtkWidget *window = create_window(params);

    params->drawing_area = create_drawing_area(params);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    g_signal_connect(window, "key-press-event", G_CALLBACK(zoom_graphic), params);
    g_signal_connect(params->drawing_area, "draw", G_CALLBACK(draw_maldebrot), params);

    gtk_container_add(GTK_CONTAINER(window), params->drawing_area);
    gtk_widget_show_all(window);

    gtk_main();

    if(params){
        free(params);
        params = NULL;
    }

    return 0;
}

static void test_mandelbrot(maldebrot_params* params){
    FILE* results = NULL;
    double run_time = 0;
    int* color_maldebrot = NULL;
#if defined (O0_opt)
    results = fopen("results/results_v1_O0.csv", "a+");
#elif defined (O1_opt)
    results = fopen("results/results_v1_O1.csv", "a+");
#elif defined (O2_opt)
    results = fopen("results/results_v1_O2.csv", "a+");
#elif defined (O3_opt)
    results = fopen("results/results_v1_O3.csv", "a+");
#endif // file choose
    timespec start = {};
    timespec end = {};
    for(int i = 0; i < COUNTER + HEAT_TESTS; i++){
        color_maldebrot = (int*)calloc(SUPER_WIDTH * SUPER_HEIGHT, sizeof(int));

        clock_gettime(CLOCK_MONOTONIC, &start);
        generate_mandelbrot(params, color_maldebrot);
        clock_gettime(CLOCK_MONOTONIC, &end);

        run_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        if(i >= HEAT_TESTS){
        #ifdef __clang__
            fprintf(results, "v1;clang++;%lg\n", run_time);
        #elif defined(__GNUC__)
            fprintf(results, "v1;g++;%lg\n", run_time);
        #endif // compilator
        }
        printf("Test %d ended\n", i);

        printf("%d\n", color_maldebrot[1]); //чтобы компилятор не оптимизировал и не убирал вызов функции

        free(color_maldebrot);
    }
    fclose(results);
}

GtkWidget *create_window(maldebrot_params* params){

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Maldebrot");

    gtk_window_set_default_size(GTK_WINDOW(window), WIDTH, HEIGHT);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    return window;
}

GtkWidget *create_drawing_area(maldebrot_params* params){

    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, WIDTH, HEIGHT);

    return drawing_area;
}

static void generate_palette(maldebrot_params* params){
    for(int i = 0; i <= RGB; i++){
        unsigned char r = (unsigned char)(i % RGB);
        unsigned char g = (unsigned char)((i * 7) % RGB);
        unsigned char b = (unsigned char)((i * 13) % RGB);
        params->PALETTE[i] = (0xFF << 24) | (r << 16) | (g << 8) | b;
    } 
}

static void generate_mandelbrot(maldebrot_params* params, int* color_maldebrot){

    double dx = DX * SCALE * params->zoom;

    for(int i_for_y = 0; i_for_y < SUPER_HEIGHT; i_for_y++){

        double y0 = ((i_for_y - (double)SUPER_HEIGHT / 2) * DY) * SCALE * params->zoom + params->center_y;
        double x0 = ((- (double)SUPER_WIDTH / 2) * DX) * SCALE * params->zoom + params->center_x + BEAUTY_COEF * params->zoom;

        for(int i_for_x = 0; i_for_x < SUPER_WIDTH; i_for_x++, x0 += dx){

            double x = x0;
            double y = y0;

            int n = 0;
            for(; n < nMax; n++){
                double x2 = x * x;
                double y2 = y * y;
                double xy = x * y;

                double R2 = x2 + y2;
                if(R2 >= Rmax) break;

                x = x2 - y2 + x0;
                y = 2*xy + y0;
            }

            color_maldebrot[i_for_x + i_for_y * SUPER_WIDTH] = params->PALETTE[n];
            
        }
    }

}

static void draw_maldebrot(GtkWidget *widget, cairo_t *cr, gpointer data){
    maldebrot_params* params = (maldebrot_params*)data;    
    int* color_maldebrot = (int*)calloc(SUPER_WIDTH * SUPER_HEIGHT, sizeof(int));

    generate_mandelbrot(params, color_maldebrot);

    cairo_surface_t* surface = cairo_image_surface_create_for_data((unsigned char*)color_maldebrot, CAIRO_FORMAT_ARGB32, SUPER_WIDTH, SUPER_HEIGHT, SUPER_WIDTH * 4);

    cairo_scale(cr, (double)WIDTH / SUPER_WIDTH, (double)HEIGHT / SUPER_HEIGHT);
    cairo_set_source_surface(cr, surface, 0, 0);
    cairo_paint(cr);

    cairo_surface_destroy(surface);
    free(color_maldebrot);
}


static void zoom_graphic(GtkWidget *widget, GdkEventKey *event, gpointer data){
    maldebrot_params* params = (maldebrot_params*)data;

    switch (event->keyval) {
        case GDK_KEY_plus: 
            params->zoom *= (1 - DELTA); 
            gtk_widget_queue_draw(params->drawing_area); 
            break;
        case GDK_KEY_minus:  
            params->zoom *= (1 + DELTA);
            gtk_widget_queue_draw(params->drawing_area);
            break;
        case GDK_KEY_Up:
            params->center_y -= DELTA * params->zoom;
            gtk_widget_queue_draw(params->drawing_area);
            break;
        case GDK_KEY_Down:
            params->center_y += DELTA * params->zoom;
            gtk_widget_queue_draw(params->drawing_area);
            break;
        case GDK_KEY_Left:
            params->center_x -= DELTA * params->zoom;
            gtk_widget_queue_draw(params->drawing_area);
            break;
        case GDK_KEY_Right:
            params->center_x += DELTA * params->zoom;
            gtk_widget_queue_draw(params->drawing_area);
            break;
        default:
            break;
    }
}
