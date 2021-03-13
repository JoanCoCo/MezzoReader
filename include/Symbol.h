/**
 * Symbol.h
 *  
 * SMII 2020/2021
 * Joan Colom Colom
 *  
 * Description of the class Symbol.
 */

#ifndef SYMBOL
#define SYMBOL

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <list>

using namespace std;
using namespace cv;

class Symbol {
private:
    string sourceTemplate;
    float scale;
    float scaleCorrectionFactor;
    double threshold;
    bool isSilence;
    int id;
public:
    Symbol(string source, float scale, float scf, double threshold, int id, bool isSilence = false);
    string get_source_template();
    float get_scale();
    float get_scale_correction_factor();
    double get_threshold();
    bool is_silence();
    int get_appropiate_pixel_scale(float ref);
    int get_id();
};

#endif