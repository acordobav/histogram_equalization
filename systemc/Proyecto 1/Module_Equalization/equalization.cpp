//-----------------------------------------------------
#include <systemc.h>

#define H_LENGTH 256

SC_MODULE (hist_equalizer) {

  //-----------Internal variables-------------------
  uint16_t img_rows, img_cols;
  uint8_t** original_img;
  uint8_t** filtered_img;

  sc_event start_t;

  // Constructor
  SC_CTOR(hist_equalizer) {
    SC_THREAD(compute);
  };

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
    uint8_t** image,
    uint32_t histogram[H_LENGTH])
  {
    uint8_t value = 0;

    for (int i = 0; i < img_rows; i++) {
      for (int j = 0; j < img_cols; j++) {
        value = image[i][j];
        histogram[value]++;

        wait(1, SC_NS);
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
      distribution[i] = addition / (img_rows * img_cols);

      wait(1, SC_NS);
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
    uint8_t** original_image,
    uint8_t** filtered_image)
  {
    uint8_t pixel = 0;
    uint8_t new_pixel = 0;

    for (int i = 0; i < img_rows; i++) {
      for (int j = 0; j < img_cols; j++) {
        pixel = original_image[i][j];

        new_pixel = distribution[pixel] * 255;

        filtered_image[i][j] = (uint8_t)floor(new_pixel);

        wait(1, SC_NS);
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
    uint16_t rows,
    uint16_t cols,
    uint8_t** original_image,
    uint8_t** filtered_image)
  {
    original_img = original_image;
    filtered_img = filtered_image;
    img_rows = rows;
    img_cols = cols;

    start_t.notify(0, SC_NS);
  }

  /**
   * Thread function that waits for the start_t event to start
   * the computation of the histogram equalization algorithm
   * 
   * @return void
   */
  void compute() {
    while(true) {
      wait(start_t);

      uint32_t histogram[H_LENGTH] = { 0 };
      float distribution[H_LENGTH] = { 0 };

      _calculate_histogram(original_img,
                            histogram);

      _cumulative_distribution(histogram,
                                distribution);

      _equalization(distribution,
                    original_img,
                    filtered_img);
    }
  }
}; // End of Module memory
