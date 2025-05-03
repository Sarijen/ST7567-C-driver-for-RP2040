#ifndef FONT_H
#define FONT_H

#include <stdint.h>

typedef struct {
  const char character;
  const uint8_t width;
  const uint8_t height;
  const uint8_t* bitmap_data;
} font_table;

#define TABLE_SIZE_10X16 87
#define TABLE_SIZE_5X8 87

extern font_table font_10x16[TABLE_SIZE_10X16];
extern font_table font_5x8[TABLE_SIZE_5X8];

// Special character required at the end of every font table
extern const uint8_t END_OF_TABLE[1];

#endif
