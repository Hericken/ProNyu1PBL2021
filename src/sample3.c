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

    int width, height;
    led_canvas_get_size(offscreen_canvas, &width, &height);



    //みなさんが書き換えるのはこれ以降の部分
    int i, j;
    img bmp_img_data;   //BMPファイルを読み込むためのimg変数

    bmp_img_data.width = 0;                 //img変数を初めて使うとき，これを付けて．
    bmp_img_data.height = 0;                //img変数を初めて使うとき，これを付けて．
    bmp_img_data.data = NULL;               //img変数を初めて使うとき，これを付けて．
    ReadBmp2("SIT.bmp", &bmp_img_data);	    //BMPファイルの読み込み．

    led_canvas_clear(offscreen_canvas);

    for (i = 0; i < 64; i++) {
        for (j = 0; j < 64; j++) {
            //img変数に読み込んだBMPファイルの色データを，offscreen_canvasに転送する
            led_canvas_set_pixel(offscreen_canvas, j, i, bmp_img_data.data[i][j].r, bmp_img_data.data[i][j].g, bmp_img_data.data[i][j].b);
        }
    }
    led_matrix_swap_on_vsync(matrix, offscreen_canvas);

    //2枚目を作る
    ReadBmp2("SITLarge.bmp", &bmp_img_data);	    //BMPファイルの読み込み．bmp_img_dataは再利用
    led_canvas_clear(offscreen_canvas);

    for (i = 0; i < 64; i++) {
        for (j = 0; j < 64; j++) {
            //img変数に読み込んだBMPファイルの色データを，offscreen_canvasに転送する
            led_canvas_set_pixel(offscreen_canvas, j, i, bmp_img_data.data[i + 200][j + 200].r, bmp_img_data.data[i + 200][j + 200].g, bmp_img_data.data[i + 200][j + 200].b);
        }
    }
    led_matrix_swap_on_vsync(matrix, offscreen_canvas);
    return 0;
}
