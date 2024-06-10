#include <systemc.h>
#include "equalization.cpp"
#include<unistd.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define ROWS 323
#define COLS 434

// Function to allocate memory for a matrix and return a pointer to it
uint8_t** createMatrix(int rows, int cols) {
    uint8_t **matrix = (uint8_t **)malloc(rows * sizeof(uint8_t *));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (uint8_t *)malloc(cols * sizeof(uint8_t));
    }
    return matrix;
}

// Function to free the memory allocated for the matrix
void freeMatrix(uint8_t **matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}



int sc_main(int argc, char* argv[]) {
  uint8_t** image = createMatrix(ROWS, COLS);
  uint8_t** filtered_image = createMatrix(ROWS, COLS);
  
  // Load image
  int width, height, channels;
  uint8_t* img = stbi_load("sydney.jpg", &width, &height, &channels, 0);
  // printf("width: %u | height: %u | channels: %u\n", width, height, channels);

  // Extract the red channel
  for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
          int index = (y * width + x) * channels;
          image[y][x] = img[index]; // Red channel is the first byte in each pixel
      }
  }

  // Free the image memory
  stbi_image_free(img);

  // ---------------------------------------------------------------------

  hist_equalizer eq("eq");

  sc_start(0,SC_NS);
  cout << "@" << sc_time_stamp()<< endl;

  printf("Filtering image\n");

  eq.equalize(ROWS, COLS, image, filtered_image);
  sc_start(300, SC_US);

  cout << "@" << sc_time_stamp() <<" Terminating simulation\n" << endl;

  // ---------------------------------------------------------------------
  // Save output image
  uint8_t save_image[ROWS][COLS] = { 0 };
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      save_image[i][j] = filtered_image[i][j];
    }
  }
  stbi_write_jpg("filtered.jpg", width, height, channels, save_image, width*channels);

  freeMatrix(image, ROWS);
  freeMatrix(filtered_image, ROWS);

  return 0;
}