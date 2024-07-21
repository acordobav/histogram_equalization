#ifndef _MEMORY_MAP_H
#define _MEMORY_MAP_H

// Register Info ------------------------------------------
#define REG_WIDTH                  32
#define REG_BYTES                  4
#define TOTAL_REGS                 18 //8*sensor+8*camera+Histogram+Stadistics

// Mod: Distance ------------------------------------------
#define MASK_DISTANCE              0xFFFF
#define MASK_IS_ACTIVE             0x10000
#define MASK_RANGE                 0x60000

// Mod: Camera --------------------------------------------
#define MASK_IDENTIFIER            0xFFFF
#define MASK_CAPTURE               0x10000

// Mod: Histogram Equalization ----------------------------
#define MASK_IMAGE_ROWS           0xFFFF
#define MASK_IMAGE_COLS           0xFFFF0000

#define IMG_ROWS 323 //define provisional, despues manejado por registro
#define IMG_COLS 434 //define provisional, despues manejado por registro
#define IMG_H_LENGTH 256 //define provisional, despues manejado por registro

// Mod: Estadistics ---------------------------------------
#define MASK_FLAG_SAVE_DATA_HOUR  0x0
#define MASK_HOURS_TO_SAVE_DAY    0x3FE
#define MASK_RESET                0x400

#define HOURS_TO_SAVE_DAY          24 //define provisional, despues manejado por registro

// Registers ----------------------------------------------
//Considering 1 register per sensor (8 sensors max)
#define REG_BASE_1                 0x10000
#define REG_SENSOR                 0x4

//Considering 1 register per camera (8 cameras max)
#define REG_BASE_2                 0x10032
#define REG_CAMARA                 0x4

#define REG_HISTOGRAM              0x10064
#define REG_STADISTICS             0x10068

#define REG_BASE_3                 0x10048
#define REG_CAMARA_3               0x4
#endif
