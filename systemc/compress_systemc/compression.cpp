#include "systemc.h"

// Definición del módulo de compresión
SC_MODULE(compression) {
  
  // Parámetros internos
  const unsigned char* input_image;
  unsigned char* output_image;
  int original_width, original_height, new_width, new_height, channels;

  // Eventos de sincronización
  sc_event compress_t;

  // Constructor
  SC_HAS_PROCESS(compression);
  compression(sc_module_name name) : sc_module(name) {
    SC_THREAD(compress_image_thread);
  }

  // Método para iniciar la compresión
  void compress_image(const unsigned char* input_img, int orig_width, int orig_height,
                    unsigned char* output_img, int new_w, int new_h, int ch) {
    input_image = input_img;
    output_image = output_img;
    original_width = orig_width;
    original_height = orig_height;
    new_width = new_w;
    new_height = new_h;
    channels = ch;

    compress_t.notify();
  }

  // Hilo de procesamiento de imagen
  void compress_image_thread() {
    while (true) {
      wait(compress_t);
      float width_ratio = (float)original_width / (float)new_width;
      float height_ratio = (float)original_height / (float)new_height;

      for (int y = 0; y < new_height; y++) {
        for (int x = 0; x < new_width; x++) {
          int src_x = (int)(x * width_ratio);
          int src_y = (int)(y * height_ratio);

          for (int c = 0; c < channels; c++) {
            output_image[(y * new_width + x) * channels + c] =
              input_image[(src_y * original_width + src_x) * channels + c];
            // Simular un pequeño retraso
            wait(1, SC_NS);
          }
          // Simular un retraso adicional por cada píxel procesado
          wait(1, SC_NS);
        }
        // Simular un retraso adicional por cada fila procesada
        wait(1, SC_NS);
      }
    }
  }
};
