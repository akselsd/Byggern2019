#ifndef FSM_H_INCLUDED
#define FSM_H_INCLUDED

typedef enum game_state_enum
{
    MENU_GAMES = 0,
    MENU_CHARACTERS,
    HIGHSCORE,
    MENU_DIFFICULTY,
    PLAY,
    GAME_OVER,
} game_state;

typedef enum player_diffs_enum
{
	DIFF_EASY = 0,
	DIFF_MEDIUM,
	DIFF_HARD,
} player_diffs;

uint8_t fsm_play_game(const uint8_t player_diff);
void fsm_main_loop(void);

#endif