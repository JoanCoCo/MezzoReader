#include <iostream>

#define BLACK_TEMPLATE "templates/black2.png"
#define WHITE_TEMPLATE "templates/white2.png"
#define ROUND_TEMPLATE "templates/round.png"
#define NUMBER_OF_SYMBOLS 3

const string SYMBOL [NUMBER_OF_SYMBOLS] = {BLACK_TEMPLATE, WHITE_TEMPLATE, ROUND_TEMPLATE};
const int SCALE_CORRECTION_FACTOR [NUMBER_OF_SYMBOLS] = {1, 0, 1};
const double THRESHOLD [NUMBER_OF_SYMBOLS] = {0.79, 0.8, 0.8};