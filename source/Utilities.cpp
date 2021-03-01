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

int get_note_tone(int y, Staff staff) {
    int baseLine = staff.get_line(4);
    int spaceBetweenLines = staff.get_space_between_lines();
    return (baseLine - y) / (spaceBetweenLines / 2);
}

list<Note> Utilities::extract_notes(Mat image, Staff staff, bool verbose) {
    
    list<Note> result;
    
    int baseLine = staff.get_line(4);
    int spaceBetweenLines = staff.get_space_between_lines();
    
    if(verbose) cout << "Base line: " << baseLine << endl;
    
    Mat ellip;
    adaptiveThreshold(~image, ellip, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, -2);
    Mat squareStructure = getStructuringElement(MORPH_ELLIPSE, Size(spaceBetweenLines - 1, spaceBetweenLines - 1));

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
            
            int noteClass = get_note_tone(y, staff);
            
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

    Mat justStaffImage = Utilities::crop_staff_from_image(image, staff);

    list<Point> wp = Utilities::find_matches(justStaffImage, "templates/white.png", 0.92, spaceBetweenLines, verbose);
    for(std::list<Point>::iterator i = wp.begin(); i != wp.end(); i++) {
        Note w;
        w.x = (*i).x;
        w.y = (*i).y + staff.get_upper_limit();
        w.tone = get_note_tone(w.y, staff);
        result.push_back(w);
    }
    
    if(verbose) {
        cvtColor(justStaffImage, justStaffImage, COLOR_GRAY2BGR);
        for(std::list<Point>::iterator pw = wp.begin(); pw != wp.end(); pw++) {
            cv::circle(justStaffImage, *(pw), 10, Scalar(0,0,255), 2);
        }
        Utilities::show_wait_destroy("Found positive white notes", justStaffImage);
    
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

bool compare_points(Point a, Point b) {
    return (a.x) < (b.x);
}

list<Point> Utilities::find_matches(Mat image, string templ, double thresh, int h, bool verbose) {

    Mat temp = imread( samples::findFile(templ), IMREAD_COLOR);
    cvtColor(temp, temp, COLOR_BGR2GRAY);
    int margin = 2;
    if(h > 0) {
        resize(temp, temp, Size(temp.cols * (h + margin) / temp.rows, (h + margin)));
    }
    if(verbose) {
        cout << "Template size" << Size(temp.cols, temp.rows) << endl;
        Utilities::show_wait_destroy("Template", temp);
    }

    Mat results;
    matchTemplate(image, temp, results, TM_CCORR_NORMED);
    if(verbose) Utilities::show_wait_destroy("Matching", results);

    Mat bw;
    threshold(results, bw, thresh, 255, THRESH_BINARY);
    if(verbose) Utilities::show_wait_destroy("Matching", bw);

    double minVal; double maxVal; Point minLoc; Point maxLoc;
    minMaxLoc(bw, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

    list<Point> resultl;
    while(maxVal > 0) {
        if(verbose) cout << maxLoc << " -> " << maxVal << endl;
        resultl.push_back(maxLoc);
        bw.at<int>(maxLoc) = 0;
        minMaxLoc(bw, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
    }

    if(resultl.size() > 0) {
        list<Point> filterdResult;
        resultl.sort(compare_points);
        Point currentPoint = *(resultl.begin());
        int aux = 1;
        int dispy = temp.rows / 2;
        int dispx = temp.cols / 2;
        for(std::list<Point>::iterator i = std::next(resultl.begin(), 1); i != resultl.end(); i++) {
            if((*std::prev(i, 1)).x + 3 > (*i).x) {
                currentPoint.x = currentPoint.x + (*i).x;
                currentPoint.y = currentPoint.y + (*i).y;
                aux++;
            } else {
                currentPoint.x = (currentPoint.x / aux) + dispx;
                currentPoint.y = (currentPoint.y / aux) + dispy;
                filterdResult.push_back(currentPoint);
                currentPoint = *i;
                aux = 1;
            }
        }
        

        currentPoint.x = (currentPoint.x / aux) + dispx;
        currentPoint.y = (currentPoint.y / aux) + dispy;
        filterdResult.push_back(currentPoint);

        return filterdResult;
    } else {
        return resultl;
    }
}

Mat Utilities::crop_staff_from_image(Mat image, Staff staff, bool markIt, Mat *outMark) {
    Rect staffRect(Point(0, staff.get_upper_limit()), Point(image.cols, staff.get_lower_limit()));
    Mat staffImage = image(staffRect);
    if(markIt) cv::rectangle(*outMark, staffRect, Scalar(255,160,23), 2);
    return staffImage;
}
