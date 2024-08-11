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

//#define IMG_ROWS 323 //define provisional, despues manejado por registro
//#define IMG_COLS 434 //define provisional, despues manejado por registro
//#define IMG_H_LENGTH 256 //define provisional, despues manejado por registro

// Mod: Estadistics ---------------------------------------
#define MASK_FLAG_SAVE_DATA_HOUR  0x0
#define MASK_HOURS_TO_SAVE_DAY    0x3FE
#define MASK_RESET                0x400

#define HOURS_TO_SAVE_DAY          24 //define provisional, despues manejado por registro

// Registers ----------------------------------------------
//Considering 1 register per sensor (8 sensors max)
#define REG_BASE_1                 0x10000
//#define REG_SENSOR                 0x4

//Considering 1 register per camera (8 cameras max)
#define REG_BASE_2                 0x10000
//#define REG_CAMARA                 0x4

#define REG_HISTOGRAM              0x20064
#define REG_STADISTICS             0x20068

#define REG_BASE_3                 0x10000
//#define REG_CAMARA_3               0x4
//#endif



//////////////////////////////////////////

// Registers ----------------------------------------------
//Considering 1 register per sensor (8 sensors max)

#define REG_SENSOR0                   0x10000
#define REG_SENSOR1                   0x101F4
#define REG_SENSOR2                   0x103E8
#define REG_SENSOR3                   0x105DC
#define REG_SENSOR4                   0x107D0
#define REG_SENSOR5                   0x109C4
#define REG_SENSOR6                   0x10BB8
#define REG_SENSOR7                   0x10DAC

/*
typedef enum{
"0"=0x10000,
"1"=0x101F4,
"2"=0x103E8,
"3"=0x105DC
};*/


//#define REG_SENSOR_address                     0x10000

//#define REG_SENSOR_2                   0x10DAC

//Considering 1 register per camera (8 cameras max)
#define REG_CAMARA_0                 0x10FA0
#define REG_CAMARA_1                 0x10FA2
#define REG_CAMARA_2                 0x10FA4
#define REG_CAMARA_3                 0x10FA6
#define REG_CAMARA_4                 0x10FAF
#define REG_CAMARA_5                 0x10FB1
#define REG_CAMARA_6                 0x10FB3
#define REG_CAMARA_7                 0x10FB5

#define REG_CONTADOR 			  0x10FB7


#define REG_COLS 				  0x111AB
#define REG_ROWS  				  0x1139F
#define REG_IMG_LENGTH  			  0X11593
//#define REG_CAMARA				  0x4

//#define REG_BASE_2                 	0x10048

//#define REG_HISTOGRAM              	0x10064
//#define REG_STADISTICS             	0x10068

//#define REG_BASE_3                 	0x10048
//#define REG_CONTADOR                 	0x1119B
//#define REG_CAMARA_3               0x4
#endif
