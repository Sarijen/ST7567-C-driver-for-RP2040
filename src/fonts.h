#ifndef FONT_H
#define FONT_H

#include <stdint.h>

typedef struct {
  const char character;
  const uint8_t width;
  const uint8_t height;
  const uint8_t* bitmap_data;
} font_glyph;

#define TABLE_SIZE_10X16 93
#define TABLE_SIZE_5X8 93

extern font_glyph font_10x16[TABLE_SIZE_10X16];
extern font_glyph font_5x8[TABLE_SIZE_5X8];

// Special character required at the end of every font table
extern const uint8_t END_OF_TABLE[1];

#endif
