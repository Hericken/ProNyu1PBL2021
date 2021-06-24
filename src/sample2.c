#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include "led-matrix-c.h"
#include "mnb_bmp.h"

int main(int argc, char const **argv)
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

    int width, height;
    led_canvas_get_size(offscreen_canvas, &width, &height);



    //みなさんが書き換えるのはこれ以降の部分
    //静止画1枚を作る
    led_canvas_clear(offscreen_canvas);

    led_canvas_set_pixel(offscreen_canvas, 10, 10, 255, 0, 0); //(10,10)に赤点を打つ
    led_canvas_set_pixel(offscreen_canvas, 20, 10, 0, 255, 0); //(20,10)に緑点を打つ
    led_canvas_set_pixel(offscreen_canvas, 20, 20, 0, 0, 255); //(20,20)に青点を打つ
    led_canvas_set_pixel(offscreen_canvas, 10, 20, 255, 255, 255); //(10,20)に白点を打つ

    //canvasの内容をLEDパネルに転送
    led_matrix_swap_on_vsync(matrix, offscreen_canvas); 
    //ここまでで静止画が1枚できる

    //静止画2枚目を作る
    led_canvas_clear(offscreen_canvas);

    int i;
    for( i=0; i<width; i++){
        led_canvas_set_pixel(offscreen_canvas, i, 32, 4*i, 128, 255-4*i);
    }
    led_matrix_swap_on_vsync(matrix, offscreen_canvas);
    //ここで2枚目ができる
    sleep(3);

    return 0;
}
