#include <immintrin.h>
#include "mandelbrot_algo.h"

void generate_mandelbrot(maldebrot_params* params, int* color_maldebrot){

    __m256d vRmax = _mm256_set1_pd(Rmax);

    double dx = DX * SCALE_X * params->zoom;
    double arr_idx[4] = {0,dx,2 * dx,3 * dx};
    __m256d idx = _mm256_loadu_pd(arr_idx);

    for(int i_for_y = 0; i_for_y < SUPER_HEIGHT; i_for_y++){

        double Y0 = ((i_for_y - (double)SUPER_HEIGHT / 2) * DY) * SCALE_Y * params->zoom + params->center_y;
        double X0 = ((- (double)SUPER_WIDTH / 2) * DX) * SCALE_X * params->zoom + params->center_x + BEAUTY_COEF * params->zoom;

        __m256d y0 = _mm256_set1_pd(Y0);
        
        for(int i_for_x = 0; i_for_x < SUPER_WIDTH; i_for_x+=4, X0 += dx*4){
            __m256d x0_v = _mm256_set1_pd(X0);
            __m256d x0 = _mm256_add_pd(x0_v, idx);

            __m256d x = x0;
            __m256d y = y0;

            __m256i N = _mm256_set1_epi64x(0);
            
            for(int n = 0; n < nMax; n++){

                __m256d x2 = _mm256_mul_pd(x, x);

                __m256d y2 = _mm256_mul_pd(y, y);

                __m256d xy = _mm256_mul_pd(x, y);

                __m256d R2 = _mm256_add_pd(x2, y2);


                __m256d cmp2_d = _mm256_cmp_pd(R2, vRmax, _CMP_LE_OS);

                N = _mm256_sub_epi64(N, _mm256_castpd_si256(cmp2_d));

                int need_exit = _mm256_testz_pd(cmp2_d, cmp2_d);
                if (need_exit) break;

                x = _mm256_add_pd(_mm256_sub_pd(x2, y2), x0);
                y = _mm256_add_pd(_mm256_add_pd(xy, xy), y0);
            }

            __m128i colors_maldebrot = _mm256_i64gather_epi32(params->PALETTE, N, 4);
            _mm_storeu_si128((__m128i*)(color_maldebrot + i_for_x +  i_for_y * SUPER_WIDTH), colors_maldebrot);
            
        }
    }



}