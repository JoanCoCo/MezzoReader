#include <iostream>

#define BLACK_NOTE_TEMPLATE "templates/black2.png"
#define WHITE_NOTE_TEMPLATE "templates/white2.png"
#define ROUND_NOTE_TEMPLATE "templates/round.png"
#define BLACK_SILENCE_TEMPLATE "templates/black_silence.png"
#define WHITE_SILENCE_TEMPLATE "templates/white_silence.png"
#define ROUND_SILENCE_TEMPLATE "templates/round_silence.png"
#define NUMBER_OF_SYMBOLS 3

const string SYMBOL [6] = {BLACK_NOTE_TEMPLATE, WHITE_NOTE_TEMPLATE, ROUND_NOTE_TEMPLATE, BLACK_SILENCE_TEMPLATE, WHITE_SILENCE_TEMPLATE, ROUND_SILENCE_TEMPLATE};
const float SCALE [6] = {1.0f, 1.0f, 1.0f, 3.0f, 0.75f, 0.75f};
const int SCALE_CORRECTION_FACTOR [6] = {1, 0, 1, 0, 0, 0};
const double THRESHOLD [6] = {0.79, 0.8, 0.8, 0.9, 0.7, 0.7};
const bool IS_NOTE [6] = {true, true, true, false, false, false};