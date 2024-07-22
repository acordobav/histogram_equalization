#include <systemc.h>

SC_MODULE(interpolation) {
  // Parámetros internos
  uint8_t** input_image;
  uint8_t** output_image;
  int original_width, original_height, new_width, new_height;

  // Eventos de sincronización
  sc_event interpolate_t;

  // Constructor
  SC_HAS_PROCESS(interpolation);
  interpolation(sc_module_name name) : sc_module(name) {
      SC_THREAD(interpolate_image_thread);
  }

  // Método para iniciar la interpolación
  void interpolate_image(uint8_t** input_img, int orig_width, int orig_height,
                          uint8_t** output_img, int new_w, int new_h) {
    input_image = input_img;
    output_image = output_img;
    original_width = orig_width;
    original_height = orig_height;
    new_width = new_w;
    new_height = new_h;
    interpolate_t.notify();
  }

    //Algoritmo de interpolacion
    void bilinear_interpolation(uint8_t** input_image, uint8_t** output_image, 
                                int orig_width, int orig_height, int new_width, int new_height) {
    // Calcula las razones de escala para cada dirección
    double xRatio = static_cast<double>(orig_width) / new_width;
    double yRatio = static_cast<double>(orig_height) / new_height;

    // Recorre cada píxel de la imagen de salida
    for (int y = 0; y < new_height; ++y) {
        for (int x = 0; x < new_width; ++x) {
            // Calcula las coordenadas en la imagen original
            double src_x = x * xRatio;
            double src_y = y * yRatio;

            // Encuentra los índices de los píxeles más cercanos
            int x1 = static_cast<int>(src_x);
            int y1 = static_cast<int>(src_y);
            int x2 = std::min(x1 + 1, orig_width - 1);
            int y2 = std::min(y1 + 1, orig_height - 1);

            // Calcula las diferencias de posición
            double x_diff = src_x - x1;
            double y_diff = src_y - y1;

            // Obtén los valores de los cuatro píxeles vecinos
            uint8_t top_left = input_image[y1][x1];
            uint8_t top_right = input_image[y1][x2];
            uint8_t bottom_left = input_image[y2][x1];
            uint8_t bottom_right = input_image[y2][x2];

            // Interpola el valor del píxel en la nueva imagen
            uint8_t value = static_cast<uint8_t>(
                top_left * (1 - x_diff) * (1 - y_diff) +
                top_right * x_diff * (1 - y_diff) +
                bottom_left * (1 - x_diff) * y_diff +
                bottom_right * x_diff * y_diff
            );

            // Asigna el valor interpolado a la imagen de salida
            output_image[y][x] = value;
        }
    }
}

  // Hilo de procesamiento de imagen
  void interpolate_image_thread() {
    while (true) {
      wait(interpolate_t);

      // Llama al algoritmo de interpolación bilineal
      bilinear_interpolation(input_image, output_image, original_width, original_height, new_width, new_height);
      // Simular un pequeño retraso
      wait(1000, SC_NS);

    }
  }
};