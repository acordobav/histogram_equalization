#include <systemc.h>
#include "equalization.cpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


int sc_main(int argc, char* argv[]) {
  uint8_t image[ROWS][COLS] = { 0 };
  uint8_t filtered_image[ROWS][COLS] = { 0 };
  
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

  sc_start();
  cout << "@" << sc_time_stamp()<< endl;

  printf("Filtering image\n");

  eq.equalize(image, filtered_image);

  cout << "@" << sc_time_stamp() <<" Terminating simulation\n" << endl;

  // ---------------------------------------------------------------------
  // Save output image
  stbi_write_jpg("filtered.jpg", width, height, channels, filtered_image, width*channels);

  return 0;
}