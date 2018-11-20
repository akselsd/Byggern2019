#include <avr/io.h>
#include <avr/interrupt.h>
#include "bit_macros.h"
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

static void speaker_play_tone(uint16_t tone) // Tone is in freq (Hz)
{
    speaker_pwm_set_freq(tone);
}

void speaker_init(void)
{
    speaker_pwm_init();
    // Set up timer 5 to keep track of the beat
    
    // Set timer 5 to mode 4: CTC with OCR5A as comp reg
    CLEAR_BIT(TCCR4A, WGM40);
    CLEAR_BIT(TCCR4A, WGM41);
    SET_BIT(TCCR4B, WGM42);
    CLEAR_BIT(TCCR4B, WGM43);

    // Set prescaler of clock to be 1024
    SET_BIT(TCCR4B, CS40);
    CLEAR_BIT(TCCR4B, CS41);
    SET_BIT(TCCR4B, CS42);

    // Set normal port operation: Only want interrupt vector TIMER5_COMPA_vect
    //CLEAR_BIT(TCCR5A, COM5A1);
    //CLEAR_BIT(TCCR5A, COM5A0);


    // Enable compare interrupts 
    SET_BIT(TIMSK4, OCIE4A);

    // Set Compare register
    OCR4A = COMP_REG_SIZE;

    // Enable overflow interrupts
    SET_BIT(TIMSK4, TOIE4);

    // Start timer at 0
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

ISR(TIMER4_COMPA_vect)
{ 
  /*curr_note.time_left--;

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
  }*/
  printf("!\n");
}
