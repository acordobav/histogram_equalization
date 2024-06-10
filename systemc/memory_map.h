#ifndef _MEMORY_MAP_H
#define _MEMORY_MAP_H

// Register Info ------------------------------------------
#define REG_WIDTH 32
#define TOTAL_REGS 6

// Mod: Distance ------------------------------------------


// Mod: Camera --------------------------------------------


// Mod: LP ------------------------------------------------


// Mod: Storage -------------------------------------------


// Mod: Histogram Equalization ----------------------------
#define IMG_ROWS 323
#define IMG_COLS 434
#define IMG_H_LENGTH 256

// Mod: Estadistics ---------------------------------------
#define INDEX_FLAG_SAVE_DATA_HOUR 0
#define INDEX_HOURS_TO_SAVE_DAY 1

// Registers ----------------------------------------------
// Necesitamos setear esto segun el Memory MAP
#define REG_DISTANCE 0x0
#define REG_CAMARA   0x4
#define REG_LP       0x8
#define REG_STORAGE  0xC
#define REG_HIST     0x10
#define REG_EST      0x14

#endif
