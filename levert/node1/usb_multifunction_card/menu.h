#ifndef MENU_H
#define MENU_H
#include <stdint.h>

/* Init module */
void menu_init(void);

/* Draw list of strings on OLED */
void menu_draw_options(const char ** options, uint8_t n_options, const char * title);

/* Select an option from the OLED screen.
   @return chosen option
 */
uint8_t menu_select_option(const uint8_t n_options);

/* Display usefull game stats on the OLED */
void menu_display_game_state(const uint16_t score, const uint8_t n_lives, const char * player_diff);

/* Display game over screen */
void menu_game_over(uint8_t score);

/* Display leaderboard */
void menu_leaderboard(const char * leaderboard);

/* Transfer current score to highscore DB */
void menu_save_score(uint16_t score);

/* Load leaderboard from highscore DB */
void menu_load_leaderboard(void);

#endif
