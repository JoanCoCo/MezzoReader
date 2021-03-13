/**
 * Symbol.cpp
 *  
 * SMII 2020/2021
 * Joan Colom Colom
 *  
 * Implementation of the class described in Symbol.h.
 */

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <list>
#include "../include/Symbol.h"

using namespace std;
using namespace cv;

Symbol::Symbol(string source, float scale, float scf, double threshold, int id, bool isSilence) {
    sourceTemplate = source;
    (*this).scale = scale;
    scaleCorrectionFactor = scf;
    (*this).threshold = threshold;
    (*this).isSilence = isSilence;
    (*this).id = id;
}

string Symbol::get_source_template() { return sourceTemplate; }
float Symbol::get_scale() { return scale; }
float Symbol::get_scale_correction_factor() { return scaleCorrectionFactor; }
double Symbol::get_threshold() { return threshold; }
bool Symbol::is_silence() { return isSilence; }
int Symbol::get_id() { return id; }

int Symbol::get_appropiate_pixel_scale(float ref) {
    return ref * scale - scaleCorrectionFactor;
}