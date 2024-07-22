#include <systemc.h>
#include "camara_sensor.h"
#include <systemc-ams.h>
//#include "utils.hpp"
//#include "camara_sensor_TLM.cpp"
//#include "camara_source.cpp"

#define ROWS 323
#define COLS 434

int timer = 0; 
int count=0; 
unsigned int prueba[420546];
unsigned int analog_prueba[420546];
unsigned int digital_prueba[420546]; 
uint8_t save_image[ROWS][COLS][3] = {0};
camara_sensor::camara_sensor(sc_core::sc_module_name nm, double const_digitalization_, double tiempo_)
:
sens_active_in("sens_active_in"),
digital_image_ready("digital_image_ready"),
//sens_active
//digital_image_ready
const_digitalization(const_digitalization_)
{}


void camara_sensor::set_attributes()
{
	accept_attribute_changes();
	
	does_attribute_changes();
	
	
	set_timestep(1.0, sc_core::SC_MS);
}

void camara_sensor::change_attributes(){
	//request_next_activation(signal_in.default_event());
}

void camara_sensor::processing()
{
	int original_width, original_height, channels;
  	int index = 0;
  	const char* input_filename = "reference_image.jpg";
  	const char* output_analog_filename = "analog_image.jpg";
  	const char* output_filename = "digital_image.jpg";
  	unsigned char* original_image;
  	if (timer== 500000){
  		sc_stop();
  	
  	}
  	else{
  		timer++; 
  	}
  	//printf("Signal In"); 
  	//bool verificacion = signal_in.read();
  	
	//digital_image_ready.write(false);
	//printf("Generando imagen :D"); 
	//printf("HOla");
	//cout << "COUNT " << count << "\n";  
	
	//count++;
	//printf("Signal In in camara sensor"); 
	//cout << "channels!!" << sens_active_in.read() << "\n"; 
	if(sens_active_in.read()){
	//}*/
		//printf("Generando imagen :D"); 
	 	original_image = stbi_load(input_filename, &original_width, &original_height, &channels, 0);

	//cout << "channels!!" << sens_active_in.read() << "\n"; 
		for (int y = 0; y < (original_width*original_height*channels); y++) {
	//cout << "HOla3!!" << y << "\n";  
         	analog_prueba[y] = (static_cast<unsigned int>(original_image[y])*190); // representacion de 0 a 256, que representa 0.019, pero 19 para no trabajar con floats
         
    		}
    		stbi_write_jpg(output_analog_filename, original_width, original_height, channels, analog_prueba, 100); // Calidad
	
    		
		for (int y = 0; y < (original_width*original_height*channels); y++) {
         	digital_prueba[y] = (static_cast<unsigned int>(analog_prueba[y])/190); 
         //cout << "Listen!!" << y << "\n";  
    		} 
	
		for (int i=0; i<original_height; i++){
			for (int j=0; j<original_width; j++){
				for (int k=0; k<3; k++){
					save_image[i][j][k] = digital_prueba[index];
					index++;
				//cout << "Listen!!" << index << "\n";  
				}
	       	}
		}
	
		stbi_write_jpg(output_filename, original_width, original_height, channels, save_image, 100); // Calidad
	
	stbi_image_free(original_image);
	//free (original_image);
	//free(save_image);
  	//stbi_image_free(analog_prueba);
	//stbi_image_free(digital_prueba);
	
	digital_image_ready.write(true);
	//cout << "channels!!" << sens_active_in.read() << "\n"; 
	}
	//printf("Final :D");
	
/*	
bool read_image_ready(){    //return value 
  	return digital_image_ready;
}
*/


}



