#ifndef FSM_H_INCLUDED
#define FSM_H_INCLUDED

typedef enum game_state_enum
{
    MAIN_MENU = 0,
    LEADERBOARD,
    MENU_CHARACTERS,
    MENU_DIFFICULTY,
    PLAY,
    GAME_OVER,
    LOAD_LEADERBOARD,
} game_state;

typedef enum player_diffs_enum
{
	DIFF_EASY = 0,
	DIFF_MEDIUM,
	DIFF_HARD,
} player_diffs;

void fsm_main_loop(void);

#endif
