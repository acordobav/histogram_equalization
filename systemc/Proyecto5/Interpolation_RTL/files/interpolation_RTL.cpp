#include <iostream>
#include <cstdlib>
#include <cstring>

#define ROWS 323
#define COLS 434
#define newROWS 128
#define newCOLS 256

// Función de interpolación bilineal con tamaño fijo
void interpolate_image(const unsigned char input_image[ROWS][COLS][3], 
                       unsigned char output_image[newROWS][newCOLS][3]) {
    float width_ratio = (float)(COLS - 1) / (float)newCOLS;
    float height_ratio = (float)(ROWS - 1) / (float)newROWS;

    for (int y = 0; y < newROWS; y++) {
        for (int x = 0; x < newCOLS; x++) {
            int src_x = (int)(x * width_ratio);
            int src_y = (int)(y * height_ratio);

            for (int c = 0; c < 3; c++) {
                float dx = (float)(x * width_ratio) - src_x;
                float dy = (float)(y * height_ratio) - src_y;

                float top = input_image[src_y][src_x][c] * (1 - dx) +
                            input_image[src_y][src_x + 1][c] * dx;

                float bottom = input_image[src_y + 1][src_x][c] * (1 - dx) +
                               input_image[src_y + 1][src_x + 1][c] * dx;

                output_image[y][x][c] = static_cast<unsigned char>(top * (1 - dy) + bottom * dy);
            }
        }
    }
}
