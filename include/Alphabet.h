/**
 * Alphabet.h
 *  
 * SMII 2020/2021
 * Joan Colom Colom
 *  
 * Description of the symbols to be recognized.
 */

#ifndef ALPHABET
#define ALPHABET

#include <iostream>
#include "Symbol.h"

/// Image templates containing the different symbols.
#define CROTCHET_NOTE_TEMPLATE "templates/black2.png"
#define MINIM_NOTE_TEMPLATE "templates/white2.png"
#define SEMIBREVE_NOTE_TEMPLATE "templates/round.png"
#define CROTCHET_SILENCE_TEMPLATE "templates/black_silence.png"
#define MINIM_SILENCE_TEMPLATE "templates/white_silence.png"
#define SEMIBREVE_SILENCE_TEMPLATE "templates/round_silence.png"
#define QUAVER_TAIL_UP_TEMPLATE "templates/quaver_tail_up_template2.png"
#define QUAVER_TAIL_DOWN_TEMPLATE "templates/quaver_tail_down_template.png"
#define QUAVER_SILENCE_TEMPLATE "templates/corchea_silence.png"
#define CLAVE_DE_SOL_TEMPLATE "templates/sun_key_template.png"
#define FULL_QUAVER_UP "templates/full_quaver_up.png"
#define FULL_QUAVER_DOWN "templates/full_quaver_down.png"

/// Total number of different symbols that can be recognized.
#define NUMBER_OF_SYMBOLS 7

/// IDs for the different types of quavers.
#define QUAVER_TAIL_UP_ID 0
#define QUAVER_TAIL_DOWN_ID 0

/// IDs associated to each recognizable symbol.
enum IDs {  CROTCHET_NOTE_ID, 
            MINIM_NOTE_ID, 
            SEMIBREVE_NOTE_ID, 
            QUAVER_NOTE_ID,
            CROTCHET_SILENCE_ID, 
            MINIM_SILENCE_ID, 
            SEMIBREVE_SILENCE_ID,
            QUAVER_SILENCE_ID };

/// Description of the traits and parameters of each symbol. 
static Symbol CROTCHET_NOTE = Symbol(CROTCHET_NOTE_TEMPLATE, 1.0f, 1.0f, 0.76, CROTCHET_NOTE_ID);
static Symbol MINIM_NOTE = Symbol(MINIM_NOTE_TEMPLATE, 1.0f, 0.0f, 0.8, MINIM_NOTE_ID);
static Symbol SEMIBREVE_NOTE = Symbol(SEMIBREVE_NOTE_TEMPLATE, 1.0f, 1.0f, 0.8, SEMIBREVE_NOTE_ID);
static Symbol CROTCHET_SILENCE = Symbol(CROTCHET_SILENCE_TEMPLATE, 3.0f, 0.0f, 0.88, CROTCHET_SILENCE_ID, true);
static Symbol MINIM_SILENCE = Symbol(MINIM_SILENCE_TEMPLATE, 0.75f, 0.0f, 0.71, MINIM_SILENCE_ID, true);
static Symbol SEMIBREVE_SILENCE = Symbol(SEMIBREVE_SILENCE_TEMPLATE, 0.75f, 0.0f, 0.7, SEMIBREVE_SILENCE_ID, true);
static Symbol QUAVER_SILENCE = Symbol(QUAVER_SILENCE_TEMPLATE, 2.0f, 0.0f, 0.95, QUAVER_SILENCE_ID, true);
static Symbol CLAVE_DE_SOL = Symbol(CLAVE_DE_SOL_TEMPLATE, 7.0f, 0.0f, 0.5, -1);

static Symbol QUAVER_TAIL_UP = Symbol(QUAVER_TAIL_UP_TEMPLATE, 2.5f, 2.0f, 0.9, QUAVER_NOTE_ID);
static Symbol QUAVER_TAIL_DOWN = Symbol(FULL_QUAVER_DOWN, 4.0f, 0.0f, 0.9, QUAVER_NOTE_ID);

/// Collection of all the recognizible symbols.
static Symbol SYMBOLS[NUMBER_OF_SYMBOLS] = {CROTCHET_NOTE, 
                                            MINIM_NOTE, 
                                            SEMIBREVE_NOTE, 
                                            CROTCHET_SILENCE,
                                            MINIM_SILENCE, 
                                            SEMIBREVE_SILENCE, 
                                            QUAVER_SILENCE};

#endif