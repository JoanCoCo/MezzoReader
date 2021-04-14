/**
 * MezzoReader.cpp
 *  
 * SMII 2020/2021
 * Joan Colom Colom
 * 
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <list>
#include "../include/Staff.h"
#include "../include/MezzoUtilities.h"
#include "../include/Note.h"
#include "../include/MezzoPlayer.h"

using namespace std;
using namespace cv;

bool visualModeOn;
bool playModeOn;

bool adaptativeMode;
int expectedLines = 0;
float adaptativePresition = 0.0f;

int main(int argc, char** argv)
{

    CommandLineParser parser(argc, argv, 
        "{help h usage ? | | Usage examples: \n\t\t./MezzoReader -i=images/notes.png -o=analysed_image.png -p -v \n\t\t./MezzoReader -a=2 -ap=5}"
        "{i input | images/notation.png | Input image.}"
        "{v visual | <none> | Visual mode.}" 
        "{p play | <none> | Play music after the analysis.}" 
        "{a staffs | <none> | Adaptative mode give then number of staffs\n\t\tcontained in the image.}"
        "{n precision | 3 | Precision for the adaptative mode as 10e-n.}"
        "{o output | pentagrama_analizado.png | Output image.}");
    parser.about("Use this script to do read a music sheet from an image.");
    if (parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }
    Mat src = imread(parser.get<String>("i") , IMREAD_COLOR);
    string outName = parser.get<String>("o");
    visualModeOn = parser.has("v");
    playModeOn = parser.has("p");
    adaptativeMode = parser.has("a");
    if(adaptativeMode) {
        adaptativePresition = parser.get<float>("n");
        expectedLines = parser.get<int>("a") * 5;
    }

    if (src.empty())
    {
        cout << "Could not open or find the image!\n" << endl;
        return -1;
    }
    //imshow("src", src);

    Mat gray;
    if (src.channels() == 3)
    {
        cvtColor(src, gray, COLOR_BGR2GRAY);
    }
    else
    {
        gray = src;
    }
    //MezzoUtilities::show_wait_destroy("gray", gray);

    Mat bw;
    adaptiveThreshold(~gray, bw, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, -2);
    
    list<Staff> staffsFound;

    if(adaptativeMode) {
        cout << "Adaptative mode is on." << endl;
        cout << "The expected number of lines is " << expectedLines << "." << endl;
        cout << "The precision to be used is 10e-" << (int) adaptativePresition << "." << endl;
        staffsFound = MezzoUtilities::extract_all_staffs(bw, adaptativeMode, expectedLines, adaptativePresition);
    } else {
        staffsFound = MezzoUtilities::extract_all_staffs(bw);
    }

    cout << staffsFound.size() << " staffs have been found." << endl;

    list<Note> allNotesList = list<Note>();

    Mat results = src.clone();

    if(staffsFound.size() == 0) {
        list<int> lines = MezzoUtilities::find_horizontal_lines(bw);
        lines = MezzoUtilities::filter_horizontal_lines(lines);
        for(std::list<int>::iterator i = lines.begin(); i != lines.end(); i++) {
            Point p1(0, *i);
            Point p2(results.cols - 1, *i);
            cv::line(results, p1, p2, Scalar(19,201,198), 2);
        }
    }

    for(std::list<Staff>::iterator s = staffsFound.begin(); s != staffsFound.end(); s++) {
        for(int i = 0; i < 5; i++) {
            Point p1(0, (*s).get_line(i));
            Point p2(results.cols - 1, (*s).get_line(i));
            cv::line(results, p1, p2, Scalar(19,201,198), 2);
        }

        list<Note> notes = MezzoUtilities::extract_notes_v2(gray, *s, visualModeOn);

        for(std::list<Note>::iterator n = notes.begin(); n != notes.end(); n++) {
            MezzoUtilities::draw_note(&results, *n, *s);
            if(playModeOn) allNotesList.push_back(*n);
        }
    }

    if(playModeOn) {
        MezzoPlayer* player = new MezzoPlayer(); 

        for(std::list<Note>::iterator n = allNotesList.begin(); n != allNotesList.end(); n++) {
            Mat view = src.clone();
            MezzoUtilities::draw_note(&view, *n, *(staffsFound.begin()));
            MezzoUtilities::show_wait_time_destroy("Playing...", view);
            player->play(*n);
        }
        player->stop();
        delete player;
    }

    //Utilities::show_wait_destroy("Results", results);
    cv::imwrite(outName, results);
    /*for(std::list<Staff>::iterator s = staffsFound.begin(); s != staffsFound.end(); s++) {
        Mat staffImage = MezzoUtilities::crop_staff_from_image(results, *s);
        MezzoUtilities::show_wait_destroy("Staff", staffImage);
    }*/

    return 0;
}



