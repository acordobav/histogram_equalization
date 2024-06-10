#include <systemc.h>
#include "compression.cpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int sc_main(int argc, char* argv[]) {
  // Variables para la imagen original y redimensionada
  int original_width, original_height, channels;
  const char* input_filename = "filtered.jpg";
  //const char* input_filename = "jaguar.jpg";
  const char* output_filename = "compressed_image.jpg";

  // Cargar la imagen JPEG
  unsigned char* original_image = stbi_load(input_filename, &original_width, &original_height, &channels, 0);
  if (original_image == NULL) {
    printf("Error al cargar la imagen\n");
    return 1;
  } else {
    printf("Imagen cargada exitosamente:\n");
    printf("Input Filename: %s\n", input_filename);
    printf("Original Width: %d\n", original_width);
    printf("Original Height: %d\n", original_height);
    printf("Channels: %d\n", channels);
}

  // Definir el nuevo tamaño de la imagen
  int new_width = original_width / 2;
  int new_height = original_height / 2;

  // Crear un nuevo buffer para la imagen redimensionada
  unsigned char* compress_image = (unsigned char*)malloc(new_width * new_height * channels);

  // Crear una instancia del módulo de compresión
  compression comp("COMP");

  // Iniciar la simulación
  sc_start(0, SC_NS);
  cout << "@" << sc_time_stamp() << " Starting compression\n";

  // Llamar a la función de compresión
  comp.compress_image(original_image, original_width, original_height, compress_image, new_width, new_height, channels);

  // Esperar a que termine la compresión
  sc_start();

  // Guardar la imagen redimensionada como JPEG
  stbi_write_jpg(output_filename, new_width, new_height, channels, compress_image, 100); // Calidad

  // Liberar la memoria
  stbi_image_free(original_image);
  free(compress_image);

  cout << "@" << sc_time_stamp() << " Terminating simulation\n";

  return 0;
}
