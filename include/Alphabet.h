#ifndef ALPHABET
#define ALPHABET

#include <iostream>
#include "Symbol.h"

#define CROTCHET_NOTE_TEMPLATE "templates/black2.png"
#define MINIM_NOTE_TEMPLATE "templates/white2.png"
#define SEMIBREVE_NOTE_TEMPLATE "templates/round.png"
#define CROTCHET_SILENCE_TEMPLATE "templates/black_silence.png"
#define MINIM_SILENCE_TEMPLATE "templates/white_silence.png"
#define SEMIBREVE_SILENCE_TEMPLATE "templates/round_silence.png"
#define QUAVER_TAIL_UP_TEMPLATE "templates/quaver_tail_up_template.png"
#define QUAVER_TAIL_DOWN_TEMPLATE "templates/quaver_tail_down_template.png"
#define QUAVER_SILENCE_TEMPLATE "templates/corchea_silence.png"
#define NUMBER_OF_SYMBOLS 7

#define QUAVER_TAIL_UP_ID 0
#define QUAVER_TAIL_DOWN_ID 1

enum IDs {  CROTCHET_NOTE_ID, 
            MINIM_NOTE_ID, 
            SEMIBREVE_NOTE_ID, 
            QUAVER_NOTE_ID,
            CROTCHET_SILENCE_ID, 
            MINIM_SILENCE_ID, 
            SEMIBREVE_SILENCE_ID,
            QUAVER_SILENCE_ID };

static Symbol CROTCHET_NOTE = Symbol(CROTCHET_NOTE_TEMPLATE, 1.0f, 1.0f, 0.75, CROTCHET_NOTE_ID);
static Symbol MINIM_NOTE = Symbol(MINIM_NOTE_TEMPLATE, 1.0f, 0.0f, 0.8, MINIM_NOTE_ID);
static Symbol SEMIBREVE_NOTE = Symbol(SEMIBREVE_NOTE_TEMPLATE, 1.0f, 1.0f, 0.8, SEMIBREVE_NOTE_ID);
static Symbol CROTCHET_SILENCE = Symbol(CROTCHET_SILENCE_TEMPLATE, 3.0f, 0.0f, 0.9, CROTCHET_SILENCE_ID, true);
static Symbol MINIM_SILENCE = Symbol(MINIM_SILENCE_TEMPLATE, 0.75f, 0.0f, 0.71, MINIM_SILENCE_ID, true);
static Symbol SEMIBREVE_SILENCE = Symbol(SEMIBREVE_SILENCE_TEMPLATE, 0.75f, 0.0f, 0.7, SEMIBREVE_SILENCE_ID, true);
static Symbol QUAVER_SILENCE = Symbol(QUAVER_SILENCE_TEMPLATE, 2.0f, 0.0f, 2.0, QUAVER_SILENCE_ID, true);

static Symbol QUAVER_TAIL_UP = Symbol(QUAVER_TAIL_UP_TEMPLATE, 3.0f, 2.0f, 0.914, QUAVER_NOTE_ID);
static Symbol QUAVER_TAIL_DOWN = Symbol(QUAVER_TAIL_DOWN_TEMPLATE, 3.0f, 0.0f, 0.92, QUAVER_NOTE_ID);

static Symbol SYMBOLS[NUMBER_OF_SYMBOLS] = {CROTCHET_NOTE, 
                                            MINIM_NOTE, 
                                            SEMIBREVE_NOTE, 
                                            CROTCHET_SILENCE,
                                            MINIM_SILENCE, 
                                            SEMIBREVE_SILENCE, 
                                            QUAVER_SILENCE};

static Symbol QUAVER_TRAITS[2] = {QUAVER_TAIL_UP, QUAVER_TAIL_DOWN};

#endif