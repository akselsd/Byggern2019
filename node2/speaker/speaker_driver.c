#include <avr/io.h>
#include "system_constants.h"
#include "speaker_driver.h"
#include "speaker_pwm_driver.h"
#include "tones.h"

#define COMP_REG_SIZE 976 // Will generate interrupt approx. 16 times a second

#define MELODY mario_melody
#define TEMPO mario_tempo 

#define SILENT_BEATS 3
#define BEATS_PER_SEC 16

typedef struct note_struct {
    uint16_t n;
    uint8_t time_left;
    uint8_t beat_count;
    uint8_t silent;
} note;

static note curr_note = {0, 0, 0, 0};

/*typedef struct song_struct {
    uint8_t * melody;
    uint8_t * tempo;
} song;

static song current_song;*/

uint16_t mario_melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,
 
  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,
 
  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0,
 
  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,
 
  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0
};

int mario_tempo[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};

void speaker_init(void)
{
    speaker_pwm_init();
    // Set up timer 5 to keep track of the beat
    
    // Set timer 5 to mode 12: CTC
    SET_BIT(TCCR5A, WGM10);
    SET_BIT(TCCR5A, WGM11);
    CLEAR_BIT(TCCR5B, WGM12);
    CLEAR_BIT(TCCR5B, WGM13);

    // Set prescaler of clock to be 1024
    SET_BIT(TCCR5B, CS50);
    CLEAR_BIT(TCCR5B, CS51);
    SET_BIT(TCCR5B, CS52);

    // Set normal port operation: Only want interrupt vector TIMER5_COMPA_vect
    CLEAR_BIT(TCCR5A, COM5A1);
    CLEAR_BIT(TCCR5A, COM5A0);

    // Set Compare register
    ICR5 = COMP_REG_SIZE;

    // Enable interrupts 
    SET_BIT(TIMSK5, OCIE5A);

    // Start timer at 0
    TCNT5 = 0;
}

void speaker_play_song(void)
{
    // Reset timer
    TCNT5 = 0;

    curr_note.n = 0;
    curr_note.silent = 0;
    curr_note.beat_count = 0;
    curr_note.time_left = TEMPO[0];
    speaker_play_tone(MELODY[0]);
}

void speaker_play_tone(uint16_t tone) // Tone is in freq (Hz)
{
    speaker_pwm_set_freq(tone);
}

ISR(TIMER5_COMPA_vect)
{   
    curr_note.time_left--;

    if (curr_note.time_left) // Nothing to be done
	return;
    else // Note or silence finished 
    {
	if (!curr_note.silent) // Note finished
	{
	    curr_note.silent = 1; // Play silent note
	    curr_note.time_left = SILENT_BEATS; 
	    speaker_play_tone(0);
	    return;
	}
	else // Silence finished
	{
	    curr_note.silent = 0;
	    ++curr_note.n; // Increment to next note
	    curr_note.time_left = TEMPO[curr_note.n]; // Set new time left
	    speaker_play_tone(MELODY[curr_note.n]); // Play new tone
	    return;
	}
    }

    // Keep beat_count between 0 and 16
    if (beat_count >= BEATS_PER_SEC)
	beat_count = 0;

    printf("B: %u\n", beat_count);
    ++beat_count;
}

