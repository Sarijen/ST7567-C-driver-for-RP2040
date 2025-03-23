#ifndef FONT_H
#define FONT_H

#include <stdint.h>

typedef struct {
  char character;
  uint8_t* bitmap_data;
} font_table;

extern font_table font_5x8[86];

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

// LOWERCASE LETTERS
extern uint8_t a_5x8[5];
extern uint8_t b_5x8[5];
extern uint8_t c_5x8[5];
extern uint8_t d_5x8[5];
extern uint8_t e_5x8[5];
extern uint8_t f_5x8[5];
extern uint8_t g_5x8[5];
extern uint8_t h_5x8[5];
extern uint8_t i_5x8[5];
extern uint8_t j_5x8[5];
extern uint8_t k_5x8[5];
extern uint8_t l_5x8[5];
extern uint8_t m_5x8[5];
extern uint8_t n_5x8[5];
extern uint8_t o_5x8[5];
extern uint8_t p_5x8[5];
extern uint8_t q_5x8[5];
extern uint8_t r_5x8[5];
extern uint8_t s_5x8[5];
extern uint8_t t_5x8[5];
extern uint8_t u_5x8[5];
extern uint8_t v_5x8[5];
extern uint8_t w_5x8[5];
extern uint8_t x_5x8[5];
extern uint8_t y_5x8[5];
extern uint8_t z_5x8[5];
extern uint8_t Z_5x8[5];
extern uint8_t Z_5x8[5];

// SPECIAL
extern uint8_t angle_bracket_left_5x8[5];
extern uint8_t angle_bracket_right_5x8[5];
extern uint8_t apostrophe_5x8[5];
extern uint8_t asterisk_5x8[5];
extern uint8_t backslash_5x8[5];
extern uint8_t bracket_left_5x8[5];
extern uint8_t bracket_right_5x8[5];
extern uint8_t caret_5x8[5];
extern uint8_t colon_5x8[5];
extern uint8_t comma_5x8[5];
extern uint8_t equals_5x8[5];
extern uint8_t exclamation_mark_5x8[5];
extern uint8_t minus_5x8[5];
extern uint8_t percent_5x8[5];
extern uint8_t period_5x8[5];
extern uint8_t plus_5x8[5];
extern uint8_t question_mark_5x8[5];
extern uint8_t quotation_5x8[5];
extern uint8_t semicolon_5x8[5];
extern uint8_t slash_5x8[5];
extern uint8_t square_bracket_left_5x8[5];
extern uint8_t square_bracket_right_5x8[5];
extern uint8_t underscore_5x8[5];

#endif
