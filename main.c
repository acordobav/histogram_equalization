#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define ROWS 323
#define COLS 434
#define H_LENGTH 256

/**
 * This function obtains the histogram of an image
 * 
 * @param[in]  image     is the matrix that represents
 *                       the original image.
 * @param[out] histogram is the histogram of the image.
 * 
 * @return void
 */
void calculate_histogram(
    uint8_t image[ROWS][COLS],
    uint32_t histogram[H_LENGTH])
{
    uint8_t value = 0;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            value = image[i][j];
            histogram[value]++;
        }
    }
}

/**
 * This function takes the histogram of an image and 
 * calculates the cumulative distribution. 
 * 
 * @param[in]  histogram    is the histogram of the image.
 * @param[out] distribution is the cumulative distribution.
 * 
 * @return void
 */
void cumulative_distribution(
    uint32_t histogram[H_LENGTH],
    float distribution[H_LENGTH])
{
    float addition = 0;

    for (int i = 0; i < H_LENGTH; i++) {
        addition += histogram[i];
        distribution[i] = addition / (ROWS * COLS);
    }
}

/**
 * This function applies the Histogram Equalization algorithm
 * 
 * @param[in]  distribution   is the acumulative distribution 
 *                            of the pixel values.
 * @param[in]  original_image is the matrix that represents the
 *                            pixels of the original image.
 * @param[out] filtered_image is the result of the algorithm.
 * 
 * @return void
 */
void equalization(
    float distribution[H_LENGTH],
    uint8_t original_image[ROWS][COLS],
    uint8_t filtered_image[ROWS][COLS])
{
    uint8_t pixel = 0;
    uint8_t new_pixel = 0;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            pixel = original_image[i][j];

            new_pixel = distribution[pixel] * 255;

            filtered_image[i][j] = (uint8_t)floor(new_pixel);
        }
    }
}

void print_matrix(uint8_t image[ROWS][COLS])
{
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%u ", image[i][j]);
        }
        printf("\n");
    }
}

void print_list(uint32_t size, uint32_t list[size])
{
    for (int i = 0; i < size; i++) {
        printf("%u ", list[i]);
    }
    printf("\n");
}

void print_list2(uint32_t size, float list[size])
{
    for (int i = 0; i < size; i++) {
        printf("%f ", list[i]);
    }
    printf("\n");
}

int main() {
    uint32_t histogram[H_LENGTH] = { 0 };
    float distribution[H_LENGTH] = { 0 };
    uint8_t filtered_image[ROWS][COLS] = { 0 };
    uint8_t image[ROWS][COLS] = { 0 };

    int width, height, channels;

    // ------------------------------------------------
    // Load image
    uint8_t* img = stbi_load("sydney.jpg", &width, &height, &channels, 0);
    printf("width: %u | height: %u | channels: %u\n", width, height, channels);

    // Extract the red channel
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = (y * width + x) * channels;
            image[y][x] = img[index]; // Red channel is the first byte in each pixel
        }
    }

    // Free the image memory
    stbi_image_free(img);
    // ------------------------------------------------


    calculate_histogram(image,
                        histogram);

    cumulative_distribution(histogram,
                            distribution);

    equalization(distribution,
                 image,
                 filtered_image);

    stbi_write_jpg("image.jpg", width, height, channels, filtered_image, width*channels);


    return 0;
}