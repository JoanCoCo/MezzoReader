/**
 * Staff.h
 *  
 * SMII 2020/2021
 * Joan Colom Colom
 *  
 * Description of the class Staff.
 */

#ifndef STAFF
#define STAFF

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <list>

using namespace std;
using namespace cv;

/**
 * Class used to represent a musical staff.
 */
class Staff {

private:
    /// Array of int representing the position of its lines.
    int lines [5]; 
    /// Integer that represents the upper limit of the staff reach.
    int upperLimit;
    /// Integer that represents the lower limit of the staff reach.   
    int lowerLimit;
    /// Integer that represents the average space between lines.
    int spaceBetweenLines;
    /// Float that represents the average line width.
    float averageLineWidth;

public:

    /**
     * Constructor of the class Staff.
     * 
     * @param l the array of int representing the 
     *      position of its lines. Expected to have 5 elements.
     * @param ul optional integer that represents the upper
     *      limit of the staff reach. It is 0 by default.
     * @param ll optional integer that represents the lower
     *      limit of the staff reach. It is INT_MAX by default.
     */
    Staff(int *l, int ul = 0, int ll = INT_MAX);

    /**
     * Modifies the position of one line.
     * 
     * @param i the line to be modified.
     * @param value the new position of the line.
     */
    void set_line(int i, int value) { lines[i] = value; }

    /**
     * Gets the position of a given line.
     * 
     * @param i the line which position we want to obtain.
     * @return integer representing the position of the line.
     */
    int get_line(int i) { return lines[i]; }

    /**
     * Obtains the space between lines in pixels.
     * 
     * @return integer representing the average number of 
     *      pixels between the lines of the staff.
     */
    int get_space_between_lines();

    /**
     * Obtains the upper limit of the staff.
     * 
     * @return integer representing the upper limit.
     */
    int get_upper_limit() { return upperLimit; }

    /**
     * Obtains the lower limit of the staff.
     * 
     * @return integer representing the lower limit.
     */
    int get_lower_limit() { return lowerLimit; }

    /**
     * Obtains the average line width of the staff.
     * 
     * @return float representing the average line width.
     */
    float get_line_width() { return averageLineWidth; }

    /**
     * Modifies the average line width of the staff.
     * 
     * @param w the average width of the lines.
     */
    void set_line_width(float w) { averageLineWidth = w; }
};

#endif
