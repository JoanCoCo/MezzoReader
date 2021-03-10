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

using namespace std;
using namespace cv;

Note::Note(int x, int y, int tone, double duration, bool isSilence) {
    (*this).x = x;
    (*this).y = y;
    (*this).tone = tone;
    (*this).duration = duration;
    (*this).isSilence = isSilence;
}

void Note::play() { }

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
    switch (t)
    {
    case DO:
        name = "D";
        break;
    case RE:
        name = "R";
        break;
    case MI:
        name = "M";
        break;
    case FA:
        name = "F";
        break;
    case SOL:
        name = "Sl";
        break;
    case LA:
        name = "L";
        break;
    case SI:
        name = "S";
        break;
    default:
        break;
    }
    name.append(std::to_string(s));
    name.append(":");
    name.append(std::to_string((int) (duration * 100.0)));
    return name;
}

