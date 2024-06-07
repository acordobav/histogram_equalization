//-----------------------------------------------------
#include <systemc.h>


#define ROWS 323
#define COLS 434
#define H_LENGTH 256

SC_MODULE (hist_equalizer) {

  //-----------Internal variables-------------------

  // Constructor
  SC_CTOR(hist_equalizer) {};

  //------------Code Starts Here-------------------------

  /**
   * This function obtains the histogram of an image
   * 
   * @param[in]  image     is the matrix that represents
   *                       the original image.
   * @param[out] histogram is the histogram of the image.
   * 
   * @return void
   */
  void _calculate_histogram(
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
  void _cumulative_distribution(
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
   * This function applies the Histogram Equalization based
   * on the provided cumulative distribution
   * 
   * @param[in]  distribution   is the acumulative distribution 
   *                            of the pixel values.
   * @param[in]  original_image is the matrix that represents the
   *                            pixels of the original image.
   * @param[out] filtered_image is the result of the algorithm.
   * 
   * @return void
   */
  void _equalization(
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

  /**
   * This function applies the Histogram Equalization algorithm
   * 
   * @param[in]  original_image is the matrix that represents the
   *                            pixels of the original image.
   * @param[out] filtered_image is the result of the algorithm.
   * 
   * @return void
   */
  void equalize(
    uint8_t original_image[ROWS][COLS],
    uint8_t filtered_image[ROWS][COLS])
  {
    uint32_t histogram[H_LENGTH] = { 0 };
    float distribution[H_LENGTH] = { 0 };

    _calculate_histogram(original_image,
                         histogram);

    _cumulative_distribution(histogram,
                             distribution);

    _equalization(distribution,
                  original_image,
                  filtered_image);
  }
  
  
}; // End of Module memory
