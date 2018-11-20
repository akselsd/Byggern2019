#include <avr/io.h>
#include <avr/interrupt.h>
#include "bit_macros.h"
#include "system_constants.h"
#include "speaker_driver.h"
#include "speaker_pwm_driver.h"
#include "tones.h"

#define COMP_REG_SIZE 15000 // Will generate interrupt approx. 16 times a second

#define MELODY mario_melody
#define TEMPO mario_tempo 

#define SILENT_BEATS 3
#define BEATS_PER_SEC 16

typedef struct note_struct {
    uint16_t n;
    uint8_t time_left;
    uint8_t silent;
} note;

uint16_t beat_count = 0;
uint16_t ticks = 300;

static note curr_note = {0, 0, 0};

static uint8_t shoot = 0;

/*typedef struct song_struct {
    uint8_t * melody;
    uint8_t * tempo;
} song;

static song current_song;*/

uint16_t mario_melody[] = {
 NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0, NOTE_DS4, NOTE_CS4, NOTE_D4,
  NOTE_CS4, NOTE_DS4,
  NOTE_DS4, NOTE_GS3,
  NOTE_G3, NOTE_CS4,
  NOTE_C4, NOTE_FS4, NOTE_F4, NOTE_E3, NOTE_AS4, NOTE_A4,
  NOTE_GS4, NOTE_DS4, NOTE_B3,
  NOTE_AS3, NOTE_A3, NOTE_GS3,
  0, 0, 0
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
    speaker_pwm_set_duty_cycle(80);
    speaker_pwm_set_freq(tone);
}

static void speaker_play_silence(void)
{
    speaker_pwm_set_duty_cycle(0);
}

void speaker_init(void)
{    
    speaker_pwm_init();

    speaker_play_silence();

    cli();

    // Enable overflow interrupts
    SET_BIT(TIMSK4, TOIE4);

    // Set timer 4 to mode 4: CTC with OCR5A as comp reg
    CLEAR_BIT(TCCR4A, WGM40);
    CLEAR_BIT(TCCR4A, WGM41);
    CLEAR_BIT(TCCR4B, WGM42);
    CLEAR_BIT(TCCR4B, WGM43);

    // Set prescaler of clock to be 1024
    SET_BIT(TCCR4B, CS40);
    CLEAR_BIT(TCCR4B, CS41);
    CLEAR_BIT(TCCR4B, CS42);

    // Set normal port operation: Only want interrupt vector TIMER5_COMPA_vect
    CLEAR_BIT(TCCR4A, COM5A1);
    SET_BIT(TCCR4A, COM5A0);


    // Set Compare register
    OCR4A = 99999999;//COMP_REG_SIZE;


    sei();
}

void speaker_play_song(void)
{
    // Reset timer
    TCNT4 = 0;

    curr_note.n = 0;
    curr_note.silent = 0;
    //curr_note.time_left = TEMPO[0];
    //speaker_play_tone(MELODY[0]);
}

void speaker_play_shoot(void)
{
  shoot = 1;
}

ISR(TIMER4_OVF_vect)
{
  if (shoot)
  {
    speaker_play_tone(++ticks);

    if (ticks >= 400)
    { 
      shoot = 0;
      ticks = 300;
      speaker_play_silence();
    }
  }

  //printf("%u\n",ticks);
  

  /* CODE FOR PLAYING MUSIC
  if (ticks % 1 == 0)
  {
    if (beat_count % 4 == 0) // Silence
    {
      curr_note.n++;
      speaker_play_silence();
    }
    else if (beat_count % 4 == 1)
    {
      speaker_play_tone(MELODY[curr_note.n]);
    }

    printf("%u\n", ticks);
    ++beat_count;
  }*/
}
