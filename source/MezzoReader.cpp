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
#include "../include/Utilities.h"
#include "../include/Note.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{

    CommandLineParser parser(argc, argv, "{@input | notes.png | input image}");
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
    Utilities::show_wait_destroy("gray", gray);

    Mat bw;
    adaptiveThreshold(~gray, bw, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, -2);
    Utilities::show_wait_destroy("binary", bw);
    
    /*list<Staff> staffsFound = Utilities::extract_all_staffs(bw);

    cout << staffsFound.size() << " staffs have been found." << endl;*/

    list<Point> p = Utilities::find_matches(bw, "Plantillas/plantilla_blanca.png", 0.51);
    Mat results;
    cvtColor(bw, results, COLOR_GRAY2BGR);
    for(std::list<Point>::iterator i = p.begin(); i != p.end(); i++) {
        cv::circle(results, *i, 5, cv::Scalar(0,255,0), 1);
    }
    Utilities::show_wait_destroy("Result", results);

    //Mat v = Utilities::erase_horizontal_lines(bw, (*staffsFound.begin()).get_space_between_lines() * 4);
    //Utilities::show_wait_destroy("V", v);

    /*Mat results;
    cvtColor(bw, results, COLOR_GRAY2BGR);

    for(std::list<Staff>::iterator s = staffsFound.begin(); s != staffsFound.end(); s++) {
        for(int i = 0; i < 5; i++) {
            Point p1(0, (*s).get_line(i));
            Point p2(results.cols - 1, (*s).get_line(i));
            cv::line(results, p1, p2, cv::Scalar(0,255,255), 1);
        }
        list<Note> notes = Utilities::extract_notes(bw, *s, true);
        for(std::list<Note>::iterator n = notes.begin(); n != notes.end(); n++) {
            Point top((*n).x - (*s).get_space_between_lines(), (*n).y - (*s).get_space_between_lines());
            Point low((*n).x + (*s).get_space_between_lines(), (*n).y + (*s).get_space_between_lines());
            cv::rectangle(results, top, low, cv::Scalar(0,255,0), 2);
            cv::putText(results, (*n).get_note_name(), top + Point(0, 5 * (*s).get_space_between_lines()), FONT_HERSHEY_PLAIN, 1,  Scalar(0,255,0), 2);
        }
    }
    Utilities::show_wait_destroy("Results", results);
    cv::imwrite("pentagrama_analizado.png", results);*/

    return 0;
}



