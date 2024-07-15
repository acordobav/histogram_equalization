#ifndef UTILS_HPP
#define UTILS_HPP

#include <systemc.h>
#include <mutex>

#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/peq_with_cb_and_phase.h"

#include "./../Proyecto_1/Module_Distance/dist_calc.cpp"

#include "./../Proyecto_1/Module_Equalization/equalization.cpp"

#include "./../Proyecto_1/Module_Compression/interpolation.cpp"


#define STB_IMAGE_IMPLEMENTATION
#include "./../Proyecto_1/Module_Equalization/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./../Proyecto_1/Module_Equalization/stb_image_write.h"



// Global variables for ID counter and mutex
int idCounter = 0;
std::mutex idMutex;

// Function to generate a unique ID
int generateUniqueID() {
    std::lock_guard<std::mutex> lock(idMutex); // Lock the mutex for thread safety
    return idCounter++;
}

// User-defined extension class
struct ID_extension: tlm::tlm_extension<ID_extension> {
  ID_extension() : transaction_id(0) {}
  virtual tlm_extension_base* clone() const { // Must override pure virtual clone method
    ID_extension* t = new ID_extension;
    t->transaction_id = this->transaction_id;
    return t;
  }

  // Must override pure virtual copy_from method
  virtual void copy_from(tlm_extension_base const &ext) {
    transaction_id = static_cast<ID_extension const &>(ext).transaction_id;
  }
  unsigned int transaction_id;
};







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















#endif