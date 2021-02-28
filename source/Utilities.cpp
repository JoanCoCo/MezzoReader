/**
 * Utilities.cpp
 *  
 * SMII 2020/2021
 * Joan Colom Colom
 *  
 * Implementation of the class described in Utilities.h.
 */

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <list>
#include "../include/Utilities.h"
#include "../include/Note.h"
#include "../include/Staff.h"

using namespace std;
using namespace cv;

list<int> Utilities::find_horizontal_lines ( Mat image ) {
    Mat horizontal = image.clone();
    
    int horizontal_size = horizontal.cols / 30;
    
    // Create structure element for extracting horizontal lines through morphology operations
    Mat horizontalStructure = getStructuringElement(MORPH_RECT, Size(horizontal_size, 1));
    
    // Apply morphology operations
    erode(horizontal, horizontal, horizontalStructure, Point(-1, -1));
    dilate(horizontal, horizontal, horizontalStructure, Point(-1, -1));
    
    // Show extracted horizontal lines
    //show_wait_destroy("horizontal", horizontal);
    //! [horiz]
    
    list <int> lines;
    
    for (int i = 0; i < horizontal.rows; i++) {
        int black = 0;
        for(int j = 0; j < horizontal.cols; j++) {
            black += (int)(horizontal.at<uchar>(i,j)) / 255;
        }
        //cout << black << endl;
        if(black > horizontal.cols * 7 / 10) {
            //cout << i << endl;
            lines.push_back(i);
        }
    }
    
    return lines;
}

list<int> Utilities::filter_horizontal_lines ( list<int> lines) {
    list <int> result;
    int previous = 0;
    int j = 0;
    int aux = 0;
    for(std::list<int>::iterator i = lines.begin(); i != lines.end(); ++i) {
        previous += *i;
        aux++;
        if(std::next(i,1) == lines.end() || (*std::next(i,1)) - (*i) >= 3) {
            result.push_back(previous / aux);
            j++;
            previous = 0;
            aux = 0;
        }
    }
    return result;
}

list<Staff> Utilities::extract_all_staffs ( Mat image ) {
    list<Staff> staffs;
    list<int> lines = Utilities::find_horizontal_lines(image);
    lines = Utilities::filter_horizontal_lines(lines);
    if(lines.size() % 5 == 0) {
        int numOfStaffs = lines.size() % 5;
        int staffLines [5];
        int i = 0;
        int c = 0;
        for(std::list<int>::iterator j = lines.begin(); j != lines.end(); ++j) {
            staffLines[i] = (int) *j;
            i++;
            if(i == 5) {
                if(numOfStaffs > 1) {
                    if(c == 0) {
                        staffs.push_back(Staff(staffLines, 0, (*std::next(j, 1) - staffLines[4])/2));
                    } else if (c == numOfStaffs - 1) {
                        staffs.push_back(Staff(staffLines, (staffLines[0] - *std::prev(j, 5))/2, INT_MAX));
                    } else {
                        staffs.push_back(Staff(staffLines, (staffLines[0] - *std::prev(j, 5))/2, (*std::next(j, 1) - staffLines[4])/2));
                    }
                } else {
                    staffs.push_back(Staff(staffLines));
                }
                i = 0;
                c++;
            }
        }
    } else {
        cout << "Error when detecting the staffs, worng number of lines detected: " << lines.size() << endl;
    }
    return staffs;
}

list<Note> Utilities::extract_notes(Mat image, Staff staff, bool verbose) {
    
    list<Note> result;
    
    int baseLine = staff.get_line(4);
    int spaceBetweenLines = staff.get_space_between_lines();
    
    cout << "Base line: " << baseLine << endl;
    
    Mat ellip = image.clone();
    Mat squareStructure = getStructuringElement(MORPH_ELLIPSE, Size(spaceBetweenLines - 1, spaceBetweenLines - 1));
    Mat hole = getStructuringElement(MORPH_ELLIPSE, Size(spaceBetweenLines - 4, spaceBetweenLines - 4));
    int diffr = (squareStructure.rows - hole.rows) / 2;
    int diffc = (squareStructure.cols - hole.cols) / 2;
    for(int i = 0; i < hole.rows; i++) {
        for(int j = 0; j < hole.cols; j++) {
            squareStructure.at<uchar>(i + diffr, j + diffc) = squareStructure.at<uchar>(i + diffr, j + diffc) - hole.at<uchar>(i, j);
        }
    }

    erode(ellip, ellip, squareStructure, Point(-1, -1));
    dilate(ellip, ellip, squareStructure, Point(-1, -1));
    
    bool inNote = false;
    bool foundNewNote = false;
    int left = 0;
    int right = 0;
    int up = ellip.rows;
    int down = 0;
    bool someWhite = false;
    for(int i = 0; i < ellip.cols; i++) {
        for(int j = std::min(ellip.rows - 1, staff.get_lower_limit()); j >= std::max(0, staff.get_upper_limit()); j--) {
            int v = (int) ellip.at<uchar>(j,i);
            if(v == 255 && !foundNewNote) {
                foundNewNote = true;
                left = i;
                down = std::max(down, j);
                inNote = true;
                someWhite = true;
            } else if (v == 255 && foundNewNote && !inNote){
                inNote = true;
                someWhite = true;
                down = std::max(down, j);
            } else if(inNote && v == 0) {
                up = std::min(up, j + 1);
                inNote = false;
            }
        }
        if(!someWhite && foundNewNote) {
            foundNewNote = false;
            right = i - 1;
            int x = ((right - left) / 2) + left;
            int y = ((down - up) / 2) + up;
            
            int noteClass = (baseLine - y) / (spaceBetweenLines / 2);
            
            Note n;
            n.x = x;
            n.y = y;
            n.tone = noteClass;
            
            result.push_back(n);
            
            if(verbose) cout << "New note was found: (" << x << ", " << y << ") --> c: " << noteClass << endl;
            
            right = 0;
            left = 0;
            up = ellip.rows;
            down = 0;
        }
        inNote = false;
        someWhite = false;
    }
    
    if(verbose) { 
        Mat d;
        cvtColor(ellip, d, COLOR_GRAY2BGR);
        Point top(0, staff.get_upper_limit());
        Point low(d.cols - 1,staff.get_lower_limit());
        cv::rectangle(d, top, low, cv::Scalar(0,255,0), 3);
        show_wait_destroy("d", d);
    }
    
    return result;
}

void Utilities::show_wait_destroy(const char* winname, cv::Mat img) {
    imshow(winname, img);
    moveWindow(winname, 500, 0);
    waitKey(0);
    destroyWindow(winname);
}

Mat Utilities::erase_horizontal_lines(Mat image, int size) {
    Mat result;

    // Create structure element for extracting vertical lines through morphology operations
    Mat verticalStructure = getStructuringElement(MORPH_RECT, Size(1, size));

    // Apply morphology operations
    erode(image, result, verticalStructure, Point(-1, -1));
    dilate(result, result, verticalStructure, Point(-1, -1));

    return result;
}

list<Point> Utilities::find_matches(Mat image, string templ, float thresh) {

    Mat temp = imread( samples::findFile(templ), IMREAD_COLOR);
    cvtColor(temp, temp, COLOR_BGR2GRAY);

    Mat results;
    int r_cols = image.cols - temp.cols + 1;
    int r_rows = image.rows - temp.rows + 1;
    results.create(r_rows, r_cols, CV_32FC1);

    matchTemplate(image, temp, results, TM_SQDIFF_NORMED);
    Utilities::show_wait_destroy("Matching", results);

    //threshold(results, results, thresh, 255, CV_THRESH_BINARY);
    //Utilities::show_wait_destroy("Matching", results);

    double minVal; double maxVal; Point minLoc; Point maxLoc;
    minMaxLoc(results, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

    list<Point> resultl;
    resultl.push_back(minLoc);
    /*for(int i = 0; i < results.rows; i++) {
        for(int j = 0; j < results.cols; j++) {
            if(results.at<uchar>(i,j)) { 
                resultl.push_back(Point(j, i));
            }
        }
    }*/

    return resultl;
} 
