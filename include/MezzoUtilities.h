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
#include "Symbol.h"

using namespace std;
using namespace cv;

/**
 * Class containing utilities for the analysis of music sheets.
 */
class MezzoUtilities {

private:
    /**
     * Call back function for the UI slider.
     * 
     * @param pos new position of the slider.
     * @param userdata pointer to user's data.
     */
    static void slider_changed(int pos, void *userdata);

    /**
     * Returns the index of the most similar template to the
     * content of an image.
     * 
     * @param symbols array of the symbols we want to search in
     *      the image.
     * @param len length of the give array.
     * @param image cv::Mat image to be analyzed.
     * @param h integer reprenting the reference height for the 
     *      templates.
     * @return vector of the form (index of the template, x 
     *      position of the template in the image, y position of 
     *      the template in the image). The index returned will be
     *      -1 if none of the symbols was found.
     */ 
    static Vec3i find_most_suitable_template_from(Symbol *symbols, int len, Mat image, int h);

    /**
     * Compares two points.
     * 
     * @param a point a.
     * @param b point b.
     * @return a.x < b.x.
     */
    static bool compare_points(Point a, Point b);

    /**
     * Obtains the tone of a note given its y coordinate and the
     * staff to which it belongs.
     * 
     * @param y integer representing the y coordinate of the note.
     * @param staff Staff that contains the note.
     * @return float representing the tone of the note measures as 
     *      half spaces.
     */
    static float get_note_tone(int y, Staff staff);
    
public:

    /**
     * Processes an image for stracting horizontal lines.
     * Implemented from the code at
     * https://docs.opencv.org/master/dd/dd7/tutorial_morph_lines_detection.html.
     * 
     * @param image the cv::Mat image to be analized.
     * @param verbose optional bool to choose if we want a verbose analysis that will show 
     *      some aditional information about the matches found. It is set to false by 
     *      default.
     * @return list of integer representing the rows of the
     *      image where horizontal lines can be found.
     */
    static list<int> find_horizontal_lines ( Mat image, float percentage = 70.0f, bool verbose = false );

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
    static list<Staff> extract_all_staffs ( Mat image, bool adaptative = false, int expectedLines = 0, float precision = 3.0f );

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
     * the user presses some key. This function was extracted from the 
     * example at https://docs.opencv.org/master/dd/dd7/tutorial_morph_lines_detection.html.
     * 
     * @param winname string that represents the name of the window
     *      created.
     * @param img cv::Mat image to be displayed.
     */
    static void show_wait_destroy(const char* winname, cv::Mat img);

    /**
     * Creates a new window to display an image that will wait rertain
     * amount of time. This function was extracted from the example at
     * https://docs.opencv.org/master/dd/dd7/tutorial_morph_lines_detection.html.
     * 
     * @param winname string that represents the name of the window
     *      created.
     * @param img cv::Mat image to be displayed.
     */
    static void show_wait_time_destroy(const char* winname, cv::Mat img);

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

    /**
     * Gets the most suitable template found in the image.
     * 
     * @param image cv::Mat containing a shape to match with the templates.
     * @param h integer representing the reference height for the templates.
     * @return integer representing the template that gave a higher score, or -1 if none
     *      of the templates matched with the image provided.
     */
    static Vec3i find_most_suitable_template(Mat image, int h);

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
    static list<Note> extract_notes_v2(Mat image, Staff staff, bool visual = false);

    /**
     * Draws a note on a given image.
     * 
     * @param image pointer to the cv::Mat image to be drawn on.
     * @param note Note to be drawn.
     * @param staff Staff containing the note.
     * @param showDescription optional bool to add a text description 
     *      below the note. It is set to false by default.
     */
    static void draw_note(Mat* image, Note note, Staff staff, bool showDescription = false);
};

#endif
