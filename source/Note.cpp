/**
 * Note.cpp
 *  
 * SMII 2020/2021
 * Joan Colom Colom
 *  
 * Implementation of the class described in Note.h.
 */

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <list>
#include "../include/Note.h"
#include "../include/Alphabet.h"

using namespace std;
using namespace cv;

Note::Note(int x, int y, int tone, int symbol, bool isSilence) {
    (*this).x = x;
    (*this).y = y;
    (*this).tone = tone;
    (*this).isSilence = isSilence;
    Note::set_note_duration(symbol);
}

void Note::set_note_duration(int symbolId) {
    switch (symbolId)
    {
    case CROTCHET_NOTE_ID:
        duration = 1.0;
        break;
    case MINIM_NOTE_ID:
        duration = 2.0;
        break;
    case SEMIBREVE_NOTE_ID:
        duration = 4.0;
        break;
    case CROTCHET_SILENCE_ID:
        duration = 1.0;
        break;
    case MINIM_SILENCE_ID:
        duration = 2.0;
        break;
    case SEMIBREVE_SILENCE_ID:
        duration = 4.0;
        break;
    case QUAVER_NOTE_ID:
        duration = 0.5;
        break;
    case QUAVER_SILENCE_ID:
        duration = 0.5;
        break;
    default:
        duration = 0.0;
        break;
    }
}

string Note::get_note_name() {
    string name = "";
    int t = tone + 2;
    int s;
    if(t < 0) {
        int aux = t;
        t = 7 * (t / 7 + 1) + t;
        s = -1 - (aux / 7);
    } else {
        s = t / 7;
        t = t % 7;
    }
    if(isSilence) {
        name = "Silence";
    } else {
        switch (t)
        {
        case DO:
            name = "Do";
            break;
        case RE:
            name = "Re";
            break;
        case MI:
            name = "Mi";
            break;
        case FA:
            name = "Fa";
            break;
        case SOL:
            name = "Sol";
            break;
        case LA:
            name = "La";
            break;
        case SI:
            name = "Si";
            break;
        default:
            break;
        }
        name.append(std::to_string(s));
    }
    name.append(" : ");
    name.append(std::to_string((int) (duration * 100.0)));
    return name;
}

int* Note::tone_split(int tone) {
    int t = tone + 2;
    int s;
    if(t < 0) {
        int aux = t;
        t = 7 * (t / 7 + 1) + t;
        s = -1 - (aux / 7);
    } else {
        s = t / 7;
        t = t % 7;
    }
    static int result[2];
    result[0] = t;
    result[1] = s;
    return result;
}