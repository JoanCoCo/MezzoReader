/**
 * MezzoReader.cpp
 *  
 * SMII 2020/2021
 * Joan Colom Colom
 * 
 */

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <list>
#include "../include/Staff.h"
#include "../include/MezzoUtilities.h"
#include "../include/Note.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{

    CommandLineParser parser(argc, argv, "{@input | images/notation.png | input image}");
    Mat src = imread( samples::findFile( parser.get<String>("@input") ), IMREAD_COLOR);
    if (src.empty())
    {
        cout << "Could not open or find the image!\n" << endl;
        cout << "Usage: " << argv[0] << " <Input image>" << endl;
        return -1;
    }
    imshow("src", src);

    Mat gray;
    if (src.channels() == 3)
    {
        cvtColor(src, gray, COLOR_BGR2GRAY);
    }
    else
    {
        gray = src;
    }
    MezzoUtilities::show_wait_destroy("gray", gray);

    Mat bw;
    adaptiveThreshold(~gray, bw, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, -2);
    
    list<Staff> staffsFound = MezzoUtilities::extract_all_staffs(bw);

    cout << staffsFound.size() << " staffs have been found." << endl;

    Mat results = src.clone();

    for(std::list<Staff>::iterator s = staffsFound.begin(); s != staffsFound.end(); s++) {
        for(int i = 0; i < 5; i++) {
            Point p1(0, (*s).get_line(i));
            Point p2(results.cols - 1, (*s).get_line(i));
            cv::line(results, p1, p2, Scalar(19,201,198), 2);
        }

        list<Note> notes = MezzoUtilities::extract_notes_v2(gray, *s, false);

        for(std::list<Note>::iterator n = notes.begin(); n != notes.end(); n++) {
            Point top((*n).x - (*s).get_space_between_lines(), (*n).y - (*s).get_space_between_lines());
            Point low((*n).x + (*s).get_space_between_lines(), (*n).y + (*s).get_space_between_lines());
            //cv::putText(results, (*n).get_note_name(), top + Point(0, 5 * (*s).get_space_between_lines()), FONT_HERSHEY_PLAIN, 1,  Scalar(86,113,123), 2);
            //cv::putText(results, to_string((*n).tone), top + Point(0, 5 * (*s).get_space_between_lines()), FONT_HERSHEY_PLAIN, 1,  Scalar(86,113,123), 2);
            if(!(*n).isSilence) {
                cv::rectangle(results, top, low, Scalar(11,221,123), 2);
            } else {
                cv::rectangle(results, top, low, Scalar(86,113,123), 2);
            }
            results.at<Vec3b>(Point((*n).x, (*n).y)) = Vec3b(0, 0, 255);
        }
    }
    //Utilities::show_wait_destroy("Results", results);
    cv::imwrite("pentagrama_analizado.png", results);
    for(std::list<Staff>::iterator s = staffsFound.begin(); s != staffsFound.end(); s++) {
        Mat staffImage = MezzoUtilities::crop_staff_from_image(results, *s);
        MezzoUtilities::show_wait_destroy("Staff", staffImage);
    }

    return 0;
}



