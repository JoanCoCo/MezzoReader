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

/**
 * Class used to represent a symbol.
 */
class Symbol {
private:
    /// Path to the image template representing the symbol.
    string sourceTemplate;
    /// Height of the symbol measured in staff spaces.
    float scale;
    /// Correction factor to reduce the scale in pixels.
    float scaleCorrectionFactor;
    /// [0.0 1.0] Minimum value of similarity needed for this symbol.
    double threshold;
    /// Bool indicating if this symbol is a silence.
    bool isSilence;
    /// Symbol id.
    int id;

public:
    /**
     * Constructor of the class Note.
     * 
     * @param source path to the image template representing the symbol.
     * @param scale height of the symbol measured in staff spaces.
     * @param scf correction factor to reduce the scale in pixels.
     * @param threshold minimum value of similarity needed for this symbol.
     * @param id symbol id.
     * @param isSilence optional bool to indicate if it's a 
     *      silence. It is set to false by default.
     */
    Symbol(string source, float scale, float scf, double threshold, int id, bool isSilence = false);

    /**
     * Obtains the path to the image template representing the symbol.
     * 
     * @return string containing the path to the template.
     */
    string get_source_template();

    /**
     * Obtains the height of the symbol measured in staff spaces.
     * 
     * @return float representing the scale of the symbol.
     */
    float get_scale();

    /**
     * Obtains the correction factor to reduce the scale in pixels.
     * 
     * @return float representing the scale correction factor.
     */
    float get_scale_correction_factor();

    /**
     * Obtains the minimum value of similarity needed for this symbol.
     * 
     * @return float representing the threshold for the symbol.
     */
    double get_threshold();

    /**
     * Obtains if the symbol is a silence or not.
     * 
     * @return bool representing if the symbol is a silence.
     */
    bool is_silence();

    /**
     * Obtains the height of the symbol measured in pixels given the
     * reference staff space.
     * 
     * @param ref size of the average space between the lines of the 
     *      staff we want to analyze.
     * @return float representing the scale of the symbol.
     */
    int get_appropiate_pixel_scale(float ref);

    /**
     * Obtains the id of the symbol.
     * 
     * @return int representing the identifier of the symbol.
     */
    int get_id();
};

#endif