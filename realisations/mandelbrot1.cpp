#include "mandelbrot_algo.h"

void generate_mandelbrot(maldebrot_params* params, int* color_maldebrot){

    double dx = DX * SCALE_X * params->zoom;

    for(int i_for_y = 0; i_for_y < SUPER_HEIGHT; i_for_y++){

        double y0 = ((i_for_y - (double)SUPER_HEIGHT / 2) * DY) * SCALE_Y * params->zoom + params->center_y;
        double x0 = ((- (double)SUPER_WIDTH / 2) * DX) * SCALE_X * params->zoom + params->center_x + BEAUTY_COEF * params->zoom;

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