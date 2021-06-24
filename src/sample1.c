#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include "led-matrix-c.h"
#include "mnb_bmp.h"

int main(int argc, char const** argv)
{
    struct RGBLedMatrixOptions options;
    char rgb_sequence[] = "GBR";
    memset(&options, 0, sizeof(options));
    options.rows = 64;
    options.cols = 64;
    options.led_rgb_sequence = rgb_sequence;
    options.disable_hardware_pulsing = 1;

    // LEDディスプレイパネルを表す変数
    struct RGBLedMatrix* matrix;
    matrix = led_matrix_create_from_options(&options, &argc, (char***)&argv);
    if (matrix == NULL) {
        return 1;
    }
    // バッファ
    struct LedCanvas* offscreen_canvas;
    offscreen_canvas = led_matrix_create_offscreen_canvas(matrix);

    pthread_t recthread;
    if (pthread_create(&recthread, NULL, (void*)takeTimelapse, NULL)) {
        return -1;
    }


    //みなさんが書き換えるのはこれ以降の部分
    int i;
    for (i = 0; i < 64; ++i) {
        led_canvas_clear(offscreen_canvas);
        //(i,i)に赤い点を打つ
        led_canvas_set_pixel(offscreen_canvas, i, i, 255, 0, 0);
        led_matrix_swap_on_vsync(matrix, offscreen_canvas);
        usleep(100000);
    }
    return 0;
}
