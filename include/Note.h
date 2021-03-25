/**
 * Note.h
 *  
 * SMII 2020/2021
 * Joan Colom Colom
 *  
 * Description of the class Note.
 */

#ifndef NOTE
#define NOTE

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <list>

using namespace std;
using namespace cv;

/** 
 * Constants used to get representation of the
 * note as strings.
*/ 
#define DO 0
#define RE 1
#define MI 2
#define FA 3
#define SOL 4
#define LA 5
#define SI 6

/**
 * Class used to represent a musical note.
 */
class Note {
public:
    /// X coordinate of the position of the note.
    int x;
    /// Y coordinate of the position of the note.
    int y;
    /// Integer that represents the tone of the note measured as half-spaces.
    int tone;
    /// Double that represents the duration in time of the sound of the note.
    double duration;
    /// Bool to indicate if its a note or a silence.
    bool isSilence;
    /// List of durations of the different notes and silences.

public:
    /**
     * Constructor of the class Note.
     * 
     * @param x horizontal coordinate of the note's position.
     * @param y vertical coordinate of the note's position.
     * @param tone integer that represents the tone of the note measured 
     *      as half-spaces.
     * @param symbol integer that represents the symbol.
     * @param isSilence optional bool to indicate if its a note or a 
     *      silence. It is set to false by default.
     */
    Note(int x, int y, int tone, int symbol, bool isSilence = false);

    /**
     * Gets the note representation as a string.
     * The representation cosists on the note 
     * tone (Do, Re, Mi, etc.) plus +N or -N for
     * indicating how high or low are in comparition
     * with the base scale.
     * 
     * @return string representation of the note.
     */
    string get_note_name();

    /**
     * Sets the note duration.
     * 
     * @param symbolId integer representing the symbol of the note.
     */
    void set_note_duration(int symbolId);

    static int* tone_split(int tone);
};

#endif
