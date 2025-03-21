#ifndef FONT_H
#define FONT_H

#include <stdint.h>

typedef struct {
  char character;
  uint8_t* bitmap_data;
} font_table;

extern font_table font_5x8[37];

extern uint8_t space_5x8[5];

// NUMBERS
extern uint8_t zero_5x8[5];
extern uint8_t one_5x8[5];
extern uint8_t two_5x8[5];
extern uint8_t three_5x8[5];
extern uint8_t four_5x8[5];
extern uint8_t five_5x8[5];
extern uint8_t six_5x8[5];
extern uint8_t seven_5x8[5];
extern uint8_t eight_5x8[5];
extern uint8_t nine_5x8[5];
// UPPERCASE LETTERS

extern uint8_t A_5x8[5];
extern uint8_t B_5x8[5];
extern uint8_t C_5x8[5];
extern uint8_t D_5x8[5];
extern uint8_t E_5x8[5];
extern uint8_t F_5x8[5];
extern uint8_t G_5x8[5];
extern uint8_t H_5x8[5];
extern uint8_t I_5x8[5];
extern uint8_t J_5x8[5];
extern uint8_t K_5x8[5];
extern uint8_t L_5x8[5];
extern uint8_t M_5x8[5];
extern uint8_t N_5x8[5];
extern uint8_t O_5x8[5];
extern uint8_t P_5x8[5];
extern uint8_t Q_5x8[5];
extern uint8_t R_5x8[5];
extern uint8_t S_5x8[5];
extern uint8_t T_5x8[5];
extern uint8_t U_5x8[5];
extern uint8_t V_5x8[5];
extern uint8_t W_5x8[5];
extern uint8_t X_5x8[5];
extern uint8_t Y_5x8[5];
extern uint8_t Z_5x8[5];

#endif
