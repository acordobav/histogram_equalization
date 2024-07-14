#include <systemc.h>
#include "camara_sensor.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define ROWS 323
#define COLS 434

unsigned int prueba[420546];
unsigned int analog_prueba[420546];
unsigned int digital_prueba[420546]; 
uint8_t save_image[ROWS][COLS][3] = {0};
camara_sensor::camara_sensor(sc_core::sc_module_name nm, double const_digitalization_)
:
signal_in("signal_in"),
//sens_active
//digital_image_ready
const_digitalization(const_digitalization_)
{}

void camara_sensor::processing()
{
	int original_width, original_height, channels;
  	int index = 0;
  	const char* input_filename = "_filtered.jpg";
  	const char* output_filename = "imagen_generada.jpg";
  	unsigned char* original_image;
	digital_image_ready.write(false);
	//printf("HOla");
	/*if(signal_in.read()){*/
	 original_image = stbi_load(input_filename, &original_width, &original_height, &channels, 0);
	//printf("HOla2");
		
	//cout << "original_width!!" << original_width << "\n";  
	//cout << "original_height!!" << original_height << "\n"; 
	//cout << "channels!!" << channels << "\n"; 
	for (int y = 0; y < (original_width*original_height*channels); y++) {
	//cout << "HOla3!!" << y << "\n";  
         analog_prueba[y] = (static_cast<unsigned int>(original_image[y])*190); // representacion de 0 a 256, que representa 0.019, pero 19 para no trabajar con floats
         //cout << "Listen!!" << y << "\n";  
    }
	for (int y = 0; y < (original_width*original_height*channels); y++) {
         digital_prueba[y] = (static_cast<unsigned int>(analog_prueba[y])/190); 
         //cout << "Listen!!" << y << "\n";  
    } 
	
	for (int i=0; i<original_height; i++){
		for (int j=0; j<original_width; j++){
			for (int k=0; k<channels; k++){
				save_image[i][j][k] = digital_prueba[index];
				index++;
				//cout << "Listen!!" << index << "\n";  
			}
	        }
	}
	
	stbi_write_jpg(output_filename, original_width, original_height, channels, save_image, 100); // Calidad
	
		
	
	stbi_image_free(original_image);
  	//stbi_image_free(digital_image);
	digital_image_ready.write(true);



}



