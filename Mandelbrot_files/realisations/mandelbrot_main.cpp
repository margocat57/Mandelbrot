#include "glib.h"
#include "glibconfig.h"
#include "gtk/gtk.h"
#include <ctime>
#include <pango/pangocairo.h>
#include "mandelbrot_algo.h"

const int NUM_OF_TESTS = 100;

GtkWidget *create_window(maldebrot_params* params);

GtkWidget *create_drawing_area(maldebrot_params* params);

static void draw_maldebrot(GtkWidget *widget, cairo_t *cr, gpointer data);

static void generate_palette(maldebrot_params* params);

static void zoom_graphic(GtkWidget *widget, GdkEventKey *event, gpointer data);

static void test_mandelbrot(maldebrot_params* params, int N_tests, int heat_tests);


int main(int argc, char*argv[]){
    maldebrot_params* params = (maldebrot_params*)calloc(sizeof(maldebrot_params), 1);
    params->zoom = 1.0;
    generate_palette(params);

    int N = 0;
    int heat = 0;

    if(argc == 3){
        N = atoi(argv[1]);
        heat = atoi(argv[2]);

        #ifdef MEASURE
            test_mandelbrot(params, N, heat);
            free(params);
            return 0;
        #endif //MEASURE
    }

    gtk_init(&argc, &argv);

    GtkWidget *window = create_window(params);
    params->window = window;

    params->drawing_area = create_drawing_area(params);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    g_signal_connect(window, "key-press-event", G_CALLBACK(zoom_graphic), params);
    g_signal_connect(params->drawing_area, "draw", G_CALLBACK(draw_maldebrot), params);

    gtk_container_add(GTK_CONTAINER(window), params->drawing_area);
    gtk_widget_show_all(window);

    #ifdef MEASURE_DRAW
        test_mandelbrot(params, N, heat);
    #endif //MEASURE_DRAW

    gtk_main();

    if(params){
        free(params);
        params = NULL;
    }

    return 0;
}


static void test_mandelbrot(maldebrot_params* params, int N_tests, int heat_tests){
    params->color_maldebrot = (int*)calloc(SUPER_WIDTH * SUPER_HEIGHT, sizeof(int));
    double* tests_results = (double*)calloc(N_tests, sizeof(double));

    timespec start = {};
    timespec end = {};

    fprintf(stderr, "testing started\n");

    for(int i = 0; i < N_tests + heat_tests; i++){

        clock_gettime(CLOCK_MONOTONIC_RAW, &start);

        for(int j = 0; j < NUM_OF_TESTS; j++)
            generate_mandelbrot(params,  params->color_maldebrot);

        clock_gettime(CLOCK_MONOTONIC_RAW, &end);

        if(i >= heat_tests)
            tests_results[i - heat_tests] = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

        #ifdef MEASURE_DRAW
        gtk_widget_queue_draw(params->drawing_area);

        while(gtk_events_pending()){
            gtk_main_iteration();
        }
        #endif //MEASURE_DRAW
    }

    fprintf(stderr, "testing ended\n");

    free( params->color_maldebrot);
    params->color_maldebrot = NULL;

    for(int i = 0; i < N_tests; i++){
        printf("%lg\n", tests_results[i]);
    }

    free(tests_results);
}

GtkWidget *create_window(maldebrot_params* params){

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

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


static void draw_maldebrot(GtkWidget *widget, cairo_t *cr, gpointer data){

    maldebrot_params* params = (maldebrot_params*)data;    

    #ifndef MEASURE_DRAW
    params->color_maldebrot = (int*)calloc(SUPER_WIDTH * SUPER_HEIGHT, sizeof(int));
    generate_mandelbrot(params, params->color_maldebrot);
    #endif //MEASURE_DRAW

    cairo_surface_t* surface = cairo_image_surface_create_for_data((unsigned char*)params->color_maldebrot, CAIRO_FORMAT_ARGB32, SUPER_WIDTH, SUPER_HEIGHT, SUPER_WIDTH * 4);

    cairo_scale(cr, (double)WIDTH / SUPER_WIDTH, (double)HEIGHT / SUPER_HEIGHT);
    cairo_set_source_surface(cr, surface, 0, 0);
    cairo_paint(cr);

    cairo_surface_destroy(surface);

    #ifndef MEASURE_DRAW
    free(params->color_maldebrot);
    params->color_maldebrot = NULL;
    #endif //MEASURE_DRAW

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
