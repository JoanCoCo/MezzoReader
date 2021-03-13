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
#include "../include/MezzoUtilities.h"
#include "../include/Note.h"
#include "../include/Staff.h"
#include "../include/Alphabet.h"
#include "../include/Symbol.h"
#include <math.h>

using namespace std;
using namespace cv;

list<int> MezzoUtilities::find_horizontal_lines ( Mat image, bool verbose ) {
    Mat horizontal = image.clone();
    
    int horizontal_size = horizontal.cols / 30;
    
    // Create structure element for extracting horizontal lines through morphology operations
    Mat horizontalStructure = getStructuringElement(MORPH_RECT, Size(horizontal_size, 1));
    
    // Apply morphology operations
    erode(horizontal, horizontal, horizontalStructure, Point(-1, -1));
    dilate(horizontal, horizontal, horizontalStructure, Point(-1, -1));
    
    // Show extracted horizontal lines
    if(verbose) show_wait_destroy("horizontal", horizontal);
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

list<int> MezzoUtilities::filter_horizontal_lines ( list<int> lines) {
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

list<Staff> MezzoUtilities::extract_all_staffs ( Mat image ) {
    list<Staff> staffs;
    list<int> lines = MezzoUtilities::find_horizontal_lines(image);
    lines = MezzoUtilities::filter_horizontal_lines(lines);
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
                    staffs.push_back(Staff(staffLines, 0, image.rows - 1));
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

float get_note_tone(int y, Staff staff) {
    float baseLine = (float) staff.get_line(4);
    float spaceBetweenLines = (float) staff.get_space_between_lines();
    float remapY = (baseLine - (float) y);
    float unit = spaceBetweenLines / 2.0f;
    return remapY / unit;
}

list<Note> MezzoUtilities::extract_notes(Mat image, Staff staff, bool verbose) {
    
    list<Note> result;
    
    /*int baseLine = staff.get_line(4);
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
    }*/

    Mat justStaffImage = MezzoUtilities::crop_staff_from_image(image, staff);

    for(int i = 0; i < NUMBER_OF_SYMBOLS; i++) {
        if(verbose) cout << "Showing " << SYMBOLS[i].get_source_template() << " positive matches." << endl ;

        bool thisIsCorchera, nextIsCorchera = false;
        Symbol symbol = SYMBOLS[i];
        list<Point> wp = MezzoUtilities::find_matches(justStaffImage, 
            symbol.get_source_template(), symbol.get_threshold(), 
            symbol.get_appropiate_pixel_scale(staff.get_space_between_lines()), verbose);
        for(std::list<Point>::iterator j = wp.begin(); j != wp.end(); j++) {
            int y = (*j).y + staff.get_upper_limit();
            Note w = Note((*j).x, y, (int) roundf(get_note_tone(y, staff)), 0);

            if(symbol.get_id() == CROTCHET_NOTE_ID) {
                Point c1, c2;
                c1 = Point(w.x, 0);

                if(next(j, 1) == wp.end()) {
                    c2 = Point(std::min(w.x + staff.get_space_between_lines() * 5, justStaffImage.cols - 1), justStaffImage.rows - 1);
                } else {
                    //c2 = Point2d((*next(j, 1)).x, justStaffImage.rows - 1);
                    c2 = Point(std::min(w.x + staff.get_space_between_lines() * 5, (*next(j,1)).x), justStaffImage.rows - 1);
                }

                Rect a(c1, c2);
                //cout << a << endl;
                Mat noteArea = justStaffImage(a);
                MezzoUtilities::show_wait_destroy("Note area", noteArea);
                Mat binaryArea;
                adaptiveThreshold(~noteArea, binaryArea, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, -2);

                Mat blockStructure = getStructuringElement(MORPH_RECT, Size(binaryArea.cols, staff.get_space_between_lines() / 2 - 1));
                
                // Apply morphology operations
                erode(binaryArea, binaryArea, blockStructure, Point(-1, -1));
                dilate(binaryArea, binaryArea, blockStructure, Point(-1, -1));
                MezzoUtilities::show_wait_destroy("Lines in note's area", binaryArea);

                bool toBreak = false;
                thisIsCorchera = false;
                int whiteInLine = 0;
                for(int g = 0; g < binaryArea.rows && !toBreak; g++) {
                    for(int h = 0; h < binaryArea.cols && !toBreak; h++) {
                        whiteInLine += (int) binaryArea.at<uchar>(g, h) / 255;
                    }
                    if(((float) whiteInLine) >= 0.75f * ((float) binaryArea.cols)) {
                        nextIsCorchera = true;
                        thisIsCorchera = true;
                        toBreak = true;
                        whiteInLine = 0;
                    }
                }

                if(thisIsCorchera) {
                    //cout << "A corchera has been found." << endl;
                    w.duration = 0.25;
                } else if(nextIsCorchera) {
                    //cout << "A corchera has been found." << endl;
                    nextIsCorchera = false;
                    w.duration = 0.25;
                } else {
                    w.duration = 1.0;
                }
            } else if(symbol.get_id() == MINIM_NOTE_ID) {
                w.duration = 2.0;
            } else if(symbol.get_id() == SEMIBREVE_NOTE_ID) {
                w.duration = 4.0;
            }

            result.push_back(w);
        }

        if(verbose) {
            Mat whiteResults;
            cvtColor(justStaffImage, whiteResults, COLOR_GRAY2BGR);
            for(std::list<Point>::iterator pw = wp.begin(); pw != wp.end(); pw++) {
                cv::circle(whiteResults, *(pw), 10, Scalar(0,0,255), 2);
            }
            MezzoUtilities::show_wait_destroy("Found positive symbols", whiteResults);
        }
    }
    
    return result;
}

void MezzoUtilities::show_wait_destroy(const char* winname, cv::Mat img) {
    imshow(winname, img);
    moveWindow(winname, 500, 0);
    waitKey(0);
    destroyWindow(winname);
}

void MezzoUtilities::show_wait_time_destroy(const char* winname, cv::Mat img) {
    imshow(winname, img);
    moveWindow(winname, 500, 0);
    waitKey(1);
    destroyWindow(winname);
}

Mat MezzoUtilities::erase_horizontal_lines(Mat image, int size) {
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

list<Point> MezzoUtilities::find_matches(Mat image, string templ, double thresh, int h, bool verbose) {

    Mat temp = imread( samples::findFile(templ), IMREAD_COLOR);
    cvtColor(temp, temp, COLOR_BGR2GRAY);
    int margin = 2;
    if(h > 0) {
        resize(temp, temp, Size(temp.cols * (h + margin) / temp.rows, (h + margin)));
    }
    if(verbose) {
        cout << "Template size" << Size(temp.cols, temp.rows) << endl;
        MezzoUtilities::show_wait_destroy("Template", temp);
    }

    Mat results;
    matchTemplate(image, temp, results, TM_CCORR_NORMED);
    if(verbose) MezzoUtilities::show_wait_destroy("Matching", results);

    Mat bw;
    threshold(results, bw, thresh, 255, THRESH_BINARY);
    if(verbose) MezzoUtilities::show_wait_destroy("Matching", bw);

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

Mat MezzoUtilities::crop_staff_from_image(Mat image, Staff staff, bool markIt, Mat *outMark) {
    Rect staffRect(Point(0, staff.get_upper_limit()), Point(image.cols, staff.get_lower_limit()));
    Mat staffImage = image(staffRect);
    if(markIt) cv::rectangle(*outMark, staffRect, Scalar(255,160,23), 2);
    return staffImage;
}

Vec3i find_most_suitable_template_from(Symbol *symbols, int len, Mat image, int h) {
    double maxV = 0;
    int maxI = -1, x = -1, y = -1;
    for(int i = 0; i < len; i++) {
        Symbol symbol = symbols[i];
        Mat temp = imread( samples::findFile(symbol.get_source_template()), IMREAD_COLOR);
        cvtColor(temp, temp, COLOR_BGR2GRAY);
        int margin = 2;
        int hr = symbol.get_appropiate_pixel_scale(h);
        resize(temp, temp, Size(temp.cols * (hr + margin) / temp.rows, (hr + margin)));
        if(temp.rows <= image.rows && temp.cols <= image.cols) {
            Mat results;
            matchTemplate(image, temp, results, TM_CCORR_NORMED);
            double minVal; double maxVal; Point minLoc; Point maxLoc;
            minMaxLoc(results, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
            if(maxV < maxVal && maxVal >= symbol.get_threshold()) {
                maxV = maxVal;
                maxI = i;
                x = maxLoc.x + temp.cols / 2;
                y = maxLoc.y + temp.rows / 2;
            }
        }
    }
    return Vec3i(maxI, x, y);
}

Vec3i MezzoUtilities::find_most_suitable_template(Mat image, int h) {
    return find_most_suitable_template_from(SYMBOLS, NUMBER_OF_SYMBOLS, image, h);
}

Vec3i identify_quaver_tail(Mat image, int h) {
    return find_most_suitable_template_from(QUAVER_TRAITS, 2, image, h);
}

list<Note> MezzoUtilities::extract_notes_v2(Mat image, Staff staff, bool verbose) {
    list<Note> result;
    Mat staffImage = crop_staff_from_image(image, staff);
    Point start;
    Point end;
    start = Point(0, 0);
    end = Point(1, staffImage.rows - 1);
    bool quaverCandidate = false;
    while(end.x < staffImage.cols) {
        Mat cell = staffImage(Rect(start, end));
        if(verbose) MezzoUtilities::show_wait_time_destroy("Cell", cell);
        int templateFound, cellX, cellY;
        Vec3i findings = MezzoUtilities::find_most_suitable_template(cell, staff.get_space_between_lines());
        templateFound = findings[0];
        cellX = findings[1];
        cellY = findings[2];
        bool previousQuaverCatched = false;
        bool barMethodFailed = true;
        if(templateFound >= 0) {
            int y = cellY + staff.get_upper_limit(); 
            int x = cellX + start.x;
            Note n = Note(x, y, (int) roundf(get_note_tone(y, staff)), SYMBOLS[templateFound].get_id(), SYMBOLS[templateFound].is_silence());
            if(quaverCandidate) {
                if(verbose) cout << "Applying late quaver match method..." << endl;
                Vec3i qtail = identify_quaver_tail(cell, staff.get_space_between_lines());
                if(qtail[0] == QUAVER_TAIL_UP_ID) {
                    Note pn = result.back();
                    result.pop_back();
                    pn.set_note_duration(QUAVER_NOTE_ID);
                    result.push_back(pn);
                    if(verbose) cout << "Quaver recognized by late match method" << endl;
                    previousQuaverCatched = true;
                }
            }
            if(verbose) cout << "New symbol was fount at " << Point(x, y) << " -> " << SYMBOLS[templateFound].get_source_template() << endl; 
            if(SYMBOLS[templateFound].get_id() == CROTCHET_NOTE_ID) {
                if(quaverCandidate && !previousQuaverCatched) {
                    if(verbose) cout << "Applying quaver bar method..." << endl;

                    Mat binaryCell;
                    adaptiveThreshold(~cell, binaryCell, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, -2);

                    Mat vStructure = getStructuringElement(MORPH_RECT, Size(1, staff.get_space_between_lines() / 2 - 1));
                    Mat hStructure = getStructuringElement(MORPH_RECT, Size(binaryCell.cols / 2, staff.get_space_between_lines() / 2 - 1));
                    int d = staff.get_space_between_lines() / 2;
                    for(int i = std::max(cellY - d, 0); i < std::min(cellY + d, binaryCell.rows); i++) {
                        for(int j = std::max(cellX - d, 0); j < std::min(cellX + d, binaryCell.cols); j++) {
                            binaryCell.at<int>(i, j) = 0;
                        }
                    }
                    // Apply morphology operations
                    //erode(binaryCell, binaryCell, vStructure, Point(-1, -1));
                    //dilate(binaryCell, binaryCell, vStructure, Point(-1, -1));
                    erode(binaryCell, binaryCell, hStructure, Point(-1, -1));
                    dilate(binaryCell, binaryCell, hStructure, Point(-1, -1));
                    //MezzoUtilities::show_wait_destroy("Lines in note's area", binaryCell);

                    bool toBreak = false;
                    int whiteInLine = 0;
                    for(int g = 0; g < binaryCell.rows && !toBreak; g++) {
                        for(int h = 0; h < binaryCell.cols && !toBreak; h++) {
                            whiteInLine += (int) binaryCell.at<uchar>(g, h) / 255;
                        }
                        if(((float) whiteInLine) >= 1.5f * ((float) binaryCell.cols)) {
                            toBreak = true;
                            barMethodFailed = false;
                            Note pn = result.back();
                            result.pop_back();
                            pn.set_note_duration(QUAVER_NOTE_ID);
                            result.push_back(pn);
                            n.set_note_duration(QUAVER_NOTE_ID);
                            if(verbose) cout << "Quaver recognized by bar method" << endl;
                        }
                    }
                } else {
                    quaverCandidate = true;
                }
                
                if(barMethodFailed) {
                    if(verbose) cout << "Applying early quaver match method..." << endl;
                    Vec3i qtail = identify_quaver_tail(cell, staff.get_space_between_lines());
                    if(qtail[0] == QUAVER_TAIL_DOWN_ID) {
                        quaverCandidate = false;
                        n.set_note_duration(QUAVER_NOTE_ID);
                        if(verbose) cout << "Quaver recognized by early match method" << endl;
                    }
                }
            } else {
                if(quaverCandidate) { 
                    quaverCandidate = false;
                }
            }
            result.push_back(n);
            start.x = end.x;
        } else if(end.x > staffImage.cols - 2) {
            if(quaverCandidate) {
                if(verbose) cout << "Applying late quaver match method..." << endl;
                Vec3i qtail = identify_quaver_tail(cell, staff.get_space_between_lines());
                if(qtail[0] == QUAVER_TAIL_UP_ID) {
                    Note pn = result.back();
                    result.pop_back();
                    pn.set_note_duration(QUAVER_NOTE_ID);
                    result.push_back(pn);
                    if(verbose) cout << "Quaver recognized by late match method" << endl;
                }
            }
        }
        end.x += 1;
        if(!verbose) cout << (int)(((float) end.x) * 100.0f / ((float) staffImage.cols)) << "%" << endl;
    }
    return result;
}