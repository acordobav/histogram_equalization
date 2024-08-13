#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>
#include <cstdlib>

#define ROWS 323
#define COLS 434
#define newROWS 128
#define newCOLS 256

// Declaración de la función de interpolación
void interpolate_image(const unsigned char input_image[ROWS][COLS][3], 
                       unsigned char output_image[newROWS][newCOLS][3]);

int main() {
    // Variables para la imagen original y redimensionada
    int original_width = COLS, original_height = ROWS, channels = 3;
    const char* input_filename = "filtered.jpg";
    const char* output_filename = "compressed_image.jpg";

    // Cargar la imagen JPEG
    unsigned char* original_image = stbi_load(input_filename, &original_width, &original_height, &channels, 3);
    if (original_image == NULL) {
        std::cerr << "Error al cargar la imagen\n";
        return 1;
    } else {
        std::cout << "Imagen cargada exitosamente:\n";
        std::cout << "Input Filename: " << input_filename << "\n";
        std::cout << "Original Width: " << original_width << "\n";
        std::cout << "Original Height: " << original_height << "\n";
        std::cout << "Channels: " << channels << "\n";
    }

    // Crear buffers para la imagen original y redimensionada
    unsigned char input_image[ROWS][COLS][3];
    unsigned char output_image[newROWS][newCOLS][3];

    // Copiar la imagen original al buffer
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            for (int c = 0; c < 3; c++) {
                input_image[y][x][c] = original_image[(y * COLS + x) * 3 + c];
            }
        }
    }

    // Llamar a la función de interpolación
    interpolate_image(input_image, output_image);

    // Guardar la imagen redimensionada como JPEG
    if (!stbi_write_jpg(output_filename, newCOLS, newROWS, 3, output_image, 100)) {
        std::cerr << "Error: no se pudo guardar la imagen\n";
    }

    // Liberar la memoria
    stbi_image_free(original_image);

    std::cout << "Imagen guardada exitosamente en: " << output_filename << "\n";

    return 0;
}
