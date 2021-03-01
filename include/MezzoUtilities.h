/**
 * Utilities.h
 *  
 * SMII 2020/2021
 * Joan Colom Colom
 *  
 * Description of a class containing the various utilities
 * used for recognition and identification of the different
 * musical elements of an image.
 */

#ifndef MEZZOUTILITIES
#define MEZZOUTILITIES

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <list>
#include "Staff.h"
#include "Note.h"

using namespace std;
using namespace cv;

class MezzoUtilities {
    
public:

    /**
     * Processes an image for stracting horizontal lines.
     * Implemented from the code at
     * https://docs.opencv.org/master/dd/dd7/tutorial_morph_lines_detection.html.
     * 
     * @param image the cv::Mat image to be analized.
     * @return list of integer representing the rows of the
     *      image where horizontal lines can be found.
     */
    static list<int> find_horizontal_lines ( Mat image );

    /**
     * Process a set of horizontal lines to filter those who
     * for its proximity can be considered as the same line.
     * 
     * @param lines list of integer representing the list of
     *      positions (rows) of a set of horizontal lines.
     * @return list of integer that represents the filtered set
     *      of lines considdering that really close lines can 
     *      represent the same line.
     */
    static list<int> filter_horizontal_lines ( list<int> lines);

    /**
     * Gets all the staffs contained on an image.
     * 
     * @param image cv::Mat image to be analized.
     * @return list of the staffs found of the image.
     */
    static list<Staff> extract_all_staffs ( Mat image );

    /**
     * Gets all the notes contained on the staff of an image.
     * 
     * @param image cv::Mat image containing the staff whose notes
     *      want to be extracted.
     * @param staff staff whose notes we want to obtain.
     * @param verbose optional bool to choose if we want a verbose 
     *      analisis that will show some aditional information about
     *      the notes found. It is set to false by default.
     */
    static list<Note> extract_notes(Mat image, Staff staff, bool verbose = false);

    /**
     * Creates a new window to display an image that will wait untill
     * the user presses some key.This function was extracted from the 
     * example at https://docs.opencv.org/master/dd/dd7/tutorial_morph_lines_detection.html.
     * 
     * @param winname string that represents the name of the window
     *      created.
     * @param img cv::Mat image to be displayed.
     */
    static void show_wait_destroy(const char* winname, cv::Mat img);

    /**
     * Erases the horizontal lines of an image. Implemented from the code at
     * https://docs.opencv.org/master/dd/dd7/tutorial_morph_lines_detection.html.
     * 
     * @param image cv::Mat image to be processed.
     * @param size integer representing the width of the lines to 
     *      be erased. 
     * @return cv::Mat image result of erasing the horizontal lines
     *      from the original image.
     */
    static Mat erase_horizontal_lines(Mat image, int size);

    /**
     * Finds the positions of the image where a template can be found. Implemented
     * from the code at https://docs.opencv.org/3.4/de/da9/tutorial_template_matching.html.
     * 
     * @param image cv::Mat image in gray scale to be processed.
     * @param templ string containing the rute of the file system to the image template
     *      we want to find.
     * @param thresh threshold between 0 and 1 to be applied for the search, meaning that
     *      positives will have to be higher or equal than the threshold to be considered.
     * @param h optional positive int that is used for rescaling the template to match a 
     *      height of h pixels. When its 0, it doesn't apply rescaling. It is 0 by default.
     * @param verbose optional bool to choose if we want a verbose analysis that will show 
     *      some aditional information about the matches found. It is set to false by 
     *      default.
     * @return list of Point inside the image where the template has been found.
     */
    static list<Point> find_matches(Mat image, string templ, double thresh, int h = 0, bool verbose = false);

    /**
     * Gets a cropped image containing only the desired staff.
     * 
     * @param image original cv::Mat image containing the staff.
     * @param staff the Staff we want to obtain from the image.
     * @param markIt optional bool that will draw a blue rectangle where the crop will be
     *      applied. It is set to false by default.
     * @param outMark optional pointer to a cv::Mat image. If markIt is true, then this 
     *      image will be modified to mark with a blue rectangle where the crop will be
     *      applied.
     */
    static Mat crop_staff_from_image(Mat image, Staff staff, bool markIt = false, Mat *outMark = nullptr);
};

#endif
