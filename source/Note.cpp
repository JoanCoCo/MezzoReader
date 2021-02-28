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
    return name;
}

