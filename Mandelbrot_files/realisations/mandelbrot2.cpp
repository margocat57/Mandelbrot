#include "mandelbrot_algo.h"

void generate_mandelbrot(maldebrot_params* params, int* color_maldebrot){

    double dx = DX * SCALE_X * params->zoom;
    double temp = ((- (double)SUPER_WIDTH / 2) * DX) * SCALE_X * params->zoom + params->center_x + BEAUTY_COEF * params->zoom;

    for(int i_for_y = 0; i_for_y < SUPER_HEIGHT; i_for_y++){

        double Y0 = ((i_for_y - (double)SUPER_HEIGHT / 2) * DY) * SCALE_Y * params->zoom + params->center_y;
        double X0 = temp;

        double y0[4] = {Y0, Y0, Y0, Y0};

        for(int i_for_x = 0; i_for_x < SUPER_WIDTH; i_for_x+=4, X0 += dx*4){
            double x0[4] = {};
            for(int j = 0; j < 4; j++){x0[j]= X0 + j*dx;}

            double x[4] = {};
            for(int j = 0; j < 4; j++){x[j]= x0[j];}

            double y[4] = {};
            for(int j = 0; j < 4; j++){y[j]= y0[j];}

            int N[4] = {};

            
            for(int n = 0; n < nMax; n++){
                double x2[4] = {};
                for(int j = 0; j < 4; j++){x2[j]= x[j] * x[j];}

                double y2[4] = {};
                for(int j = 0; j < 4; j++){y2[j]= y[j] * y[j];}

                double xy[4] = {};
                for(int j = 0; j < 4; j++){xy[j]= x[j] * y[j];}

                double R2[4] = {};
                for(int j = 0; j < 4; j++){R2[j]= x2[j] + y2[j];}

                int cmp2[4] = {};
                for(int j = 0; j < 4; j++){if(R2[j] <= Rmax){cmp2[j] = 1;}}

                for(int j = 0; j < 4; j++){N[j] += cmp2[j];}

                int mask = 0;
                for(int j = 0; j < 4; j++){mask |= (cmp2[j] << j);}
                if(!mask) break;

                for(int j = 0; j < 4; j++){x[j] = x2[j] - y2[j] + x0[j];}
                for(int j = 0; j < 4; j++){y[j] = xy[j] + xy[j] + y0[j];}
            }

            for(int j = 0; j < 4; j++){color_maldebrot[i_for_x + j + i_for_y * SUPER_WIDTH] = params->PALETTE[N[j]];}
            
        }
    }

}