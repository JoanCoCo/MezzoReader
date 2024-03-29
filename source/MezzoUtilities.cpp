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
#include <iomanip>

using namespace std;
using namespace cv;

static const int MAX_DELAY = 1000;
int speed = MAX_DELAY - 1;

list<int> MezzoUtilities::find_horizontal_lines ( Mat image, float percentage, bool verbose ) {
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
    // -----------------------------------------------------------------
    for (int i = 0; i < horizontal.rows; i++) {
        int black = 0;
        for(int j = 0; j < horizontal.cols; j++) {
            black += (int)(horizontal.at<uchar>(i,j)) / 255;
        }
        //cout << black << endl;
        if((float)black > (float)horizontal.cols * percentage / 100.0f) {
            //cout << i << endl;
            lines.push_back(i);
        }
    }
    // -----------------------------------------------------------------
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

list<Staff> MezzoUtilities::extract_all_staffs ( Mat image , bool adaptive, int expectedLines, float precision ) {
    list<Staff> staffs;
    list<int> lines;
    float percent = 0.0f;

    if(adaptive) {
        percent = 100.0f;
    } else {
        percent = 70.0f;
    }
    
    int originalNumberOfLines = 1;
    float averageLineWidth = 1.0f;

    int fastSteps = 2;
    bool endAdaptiveBehaviour = false;
    do {
        if(adaptive) {
            switch (fastSteps)
            {
            case 2:
                percent /= 2.0f;
                break;
            case 1:
                percent -= 1.0f;
                break;
            default:
                percent -= pow(10.0f, -1.0f * precision);
                break;
            }
            cout << setw(15) << '\r' << flush;
            cout << percent << "%" << '\r' << flush;
        }
        lines.clear();

        lines = MezzoUtilities::find_horizontal_lines(image, percent);
        originalNumberOfLines = lines.size();
        lines = MezzoUtilities::filter_horizontal_lines(lines);
        averageLineWidth = (float) originalNumberOfLines / (float) lines.size();

        endAdaptiveBehaviour = lines.size() == expectedLines || percent < 1.0f;

        if(adaptive && !endAdaptiveBehaviour) {
            if(lines.size() > expectedLines && fastSteps > 0) {
                if(fastSteps == 2) {
                    percent *= 2.0f;
                } else if(fastSteps == 1) {
                    percent += 1.0f;
                }
                fastSteps--;
            } else if(fastSteps == 0) {
                endAdaptiveBehaviour = lines.size() >= expectedLines;
            }
        }
    } while(adaptive && !endAdaptiveBehaviour);

    if(adaptive) {
        cout << endl;
        (lines.size() != expectedLines) ? 
            cout << "Adaptative process failed with " << percent << "%." << endl : 
            cout << "Adaptative process succed with " << percent << "%." << endl;
    }

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
                        Staff stf = Staff(staffLines, 0, (*std::next(j, 1) - staffLines[4])/2);
                        stf.set_line_width(averageLineWidth);
                        staffs.push_back(stf);
                    } else if (c == numOfStaffs - 1) {
                        Staff stf = Staff(staffLines, (staffLines[0] - *std::prev(j, 5))/2, image.rows - 1);
                        stf.set_line_width(averageLineWidth);
                        staffs.push_back(stf);
                    } else {
                        Staff stf = Staff(staffLines, (staffLines[0] - *std::prev(j, 5))/2, (*std::next(j, 1) - staffLines[4])/2);
                        stf.set_line_width(averageLineWidth);
                        staffs.push_back(stf);
                    }
                } else {
                    Staff stf = Staff(staffLines, 0, image.rows - 1);
                    stf.set_line_width(averageLineWidth);
                    staffs.push_back(stf);
                }
                i = 0;
                c++;
            }
        }
    } else {
        cout << "Error when detecting the staffs, wrong number of lines detected: " << lines.size() << endl;
    }
    return staffs;
}

float MezzoUtilities::get_note_tone(int y, Staff staff) {
    float baseLine = (float) staff.get_line(4);
    float spaceBetweenLines = (float) staff.get_space_between_lines();
    float remapY = (baseLine - (float) y);
    float unit = spaceBetweenLines / 2.0f;
    return remapY / unit - (signbit(remapY) * -1) * 0.1f;
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
                adaptiveThreshold(noteArea, binaryArea, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, -2);

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
    namedWindow(winname, WINDOW_AUTOSIZE);
    imshow(winname, img);
#if __APPLE__
    moveWindow(winname, 500, 0);
#else
    moveWindow(winname, 0, 0);
#endif
    waitKey(0);
    destroyWindow(winname);
}

void MezzoUtilities::show_wait_time_destroy(const char* winname, cv::Mat img) {
    namedWindow(winname, WINDOW_AUTOSIZE);
    imshow(winname, img);
#if __APPLE__
    moveWindow(winname, 500, 0);
    waitKey(1);
#else
    moveWindow(winname, 0, 0);
    waitKey(10);
#endif
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

bool MezzoUtilities::compare_points(Point a, Point b) {
    return (a.x) < (b.x);
}

list<Point> MezzoUtilities::find_matches(Mat image, string templ, double thresh, int h, bool verbose) {

    Mat temp = imread(templ, IMREAD_COLOR);
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

Vec3i MezzoUtilities::find_most_suitable_template_from(Symbol *symbols, int len, Mat image, int h) {
    double maxV = 0;
    int maxI = -1, x = -1, y = -1;
    for(int i = 0; i < len; i++) {
        Symbol symbol = symbols[i];
        Mat temp = imread(symbol.get_source_template(), IMREAD_COLOR);
        cvtColor(temp, temp, COLOR_BGR2GRAY);
        int margin = 2;
        int hr = symbol.get_appropiate_pixel_scale(h);
        resize(temp, temp, Size(temp.cols * (hr + margin) / temp.rows, (hr + margin))/*, 0.0, 0.0, INTER_AREA*/);
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

void MezzoUtilities::slider_changed(int pos, void *userdata) {
    speed = pos;
}

list<Note> MezzoUtilities::extract_notes_v2(Mat image, Staff staff, bool visual) {
    list<Note> result;
    Mat staffImage = crop_staff_from_image(image, staff);
    Point start;
    Point end;
    start = Point(0, 0);
    end = Point(1, staffImage.rows - 1);
    bool sliderIsCreated = false;
    int barAreaHeight = (int)(roundf(0.8005 * logf(staff.get_space_between_lines()) + 1.6667f));
    float lineSpaceRatio = staff.get_line_width() / (float) staff.get_space_between_lines();
    //cout << "Line width: " << staff.get_line_width() << endl;
    //cout << "Bar area height: " << barAreaHeight << endl;
    //cout << "Space between lines: " << staff.get_space_between_lines() << endl;
    //cout << "Ratio: " << lineSpaceRatio << endl;
    while(end.x < staffImage.cols) {
        if(visual) {
            Mat gOut = image.clone();
            cvtColor(gOut, gOut, COLOR_GRAY2BGR);
            cv::rectangle(gOut, 
                Rect(Point(start.x, staff.get_upper_limit()), Point(end.x, staff.get_lower_limit())), 
                Scalar(0,213,143), 2);
            if(result.size() > 0) {
                Note lastN = result.back();
                MezzoUtilities::draw_note(&gOut, lastN, staff, true);
            }
            if(!sliderIsCreated) {
                createTrackbar("Speed", "Reading...", &speed, MAX_DELAY - 1, slider_changed);
                sliderIsCreated = true;
            }
            imshow("Reading...", gOut);
            waitKey(MAX_DELAY - speed);
        }

        Mat cell = staffImage(Rect(start, end));
        int templateFound, cellX, cellY;
        Vec3i findings = MezzoUtilities::find_most_suitable_template(cell, staff.get_space_between_lines());
        templateFound = findings[0];
        cellX = findings[1];
        cellY = findings[2];
        if(templateFound >= 0) {
            if(SYMBOLS[templateFound].get_id() >= 0) {
                int y = cellY + staff.get_upper_limit(); 
                int x = cellX + start.x;
                //cout << "Note tone: " << get_note_tone(y, staff) << endl;
                Note n = Note(x, y, (int) roundl(get_note_tone(y, staff)), SYMBOLS[templateFound].get_id(), SYMBOLS[templateFound].is_silence());
                bool isFalsePositive = false;
                if(SYMBOLS[templateFound].get_id() == CROTCHET_NOTE_ID) {
                    // First we check if this crotchet candidate is really an isolated quaver.
                    int avSp = staff.get_space_between_lines();
                    Rect interestRect = Rect(Point(std::max(start.x, n.x - 2 * avSp), start.y), Point(std::min(staffImage.cols - 1, n.x + 2 * avSp), end.y));
                    Mat interestZone = staffImage(interestRect);
                    //MezzoUtilities::show_wait_destroy("h", interestZone);
                    Symbol symSet[] = { QUAVER_TAIL_DOWN, CLAVE_DE_SOL };
                    Vec3i qtail = find_most_suitable_template_from(symSet, 1, interestZone, staff.get_space_between_lines());
                    float blackAmount = 0;
                    switch (qtail[0])
                    {
                    case 1:
                        cout << "Clave de Sol" << endl;
                        isFalsePositive = true;
                        break;
                    case QUAVER_TAIL_DOWN_ID:
                        n.set_note_duration(QUAVER_NOTE_ID);
                        break;
                    default:
                        Mat binaryCell;
                        adaptiveThreshold(~interestZone, binaryCell, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, -2);
                        if(lineSpaceRatio > 0.2f) {
                            for(int i = 0; i < 5; i++) {
                                cv::line(binaryCell, Point(0, staff.get_line(i) - staff.get_upper_limit()), 
                                Point(binaryCell.cols - 1, staff.get_line(i) - staff.get_upper_limit()), 
                                Scalar(1, 1, 1), std::max((int) 1, 0));
                            }
                        }
                        Mat hStructure = getStructuringElement(MORPH_RECT, Size(binaryCell.cols / 2, barAreaHeight /*staff.get_space_between_lines() / 2 - 1*/));
                        //MezzoUtilities::show_wait_destroy("b", binaryCell);
                        erode(binaryCell, binaryCell, hStructure, Point(-1, -1));
                        dilate(binaryCell, binaryCell, hStructure, Point(-1, -1));
                        //MezzoUtilities::show_wait_destroy("b", binaryCell);
                        bool toBreak = false;
                        int whiteInLine = 0;
                        for(int g = 0; g < binaryCell.rows && !toBreak; g++) {
                            for(int h = 0; h < binaryCell.cols && !toBreak; h++) {
                                whiteInLine += (int) binaryCell.at<uchar>(g, h) / 255;
                            }
                            if(((float) whiteInLine) >= 0.2f * ((float) binaryCell.cols)) {
                                toBreak = true;
                                n.set_note_duration(QUAVER_NOTE_ID);
                            }
                        }
                        if(toBreak) break;
                        
                        Rect newRect = Rect(Point(n.x, std::max(0, cellY - 4 * avSp)), Point(std::min(staffImage.cols - 1, n.x + 2 * avSp), cellY));
                        Mat newInterestZone = staffImage(newRect);
                        Symbol symSet2[] = { QUAVER_TAIL_UP };
                        Vec3i rqTail = find_most_suitable_template_from(symSet2, 1, newInterestZone, staff.get_space_between_lines());
                        if(rqTail[0] == QUAVER_TAIL_UP_ID) {
                            n.set_note_duration(QUAVER_NOTE_ID);
                        }
                        break;
                    }
                }    
                if(!isFalsePositive) result.push_back(n);
            }
            start.x = end.x;
        }
        end.x += 5;
        cout << (int)(((float) end.x) * 100.0f / ((float) staffImage.cols)) << "%" << '\r' << flush;
    }
    cout << endl;
    return result;
}

void MezzoUtilities::draw_note(Mat* image, Note note, Staff staff, bool showDescription) {
    Point top(note.x - staff.get_space_between_lines(), note.y - staff.get_space_between_lines());
    Point low(note.x + staff.get_space_between_lines(), note.y + staff.get_space_between_lines());
    if(showDescription) {
        cv::putText(*image, note.get_note_name(), top + Point(0, 5 * staff.get_space_between_lines()), FONT_HERSHEY_PLAIN, 2,  Scalar(103,113,123), 2);
        //cv::putText(*image, to_string(note.tone), top + Point(0, 5 * staff.get_space_between_lines()), FONT_HERSHEY_PLAIN, 1,  Scalar(86,113,123), 2);
    }
    if(note.isSilence) {
        cv::rectangle(*image, top, low, Scalar(86,113,193), 2);
    } else if(note.duration < 1) {
        cv::rectangle(*image, top, low, Scalar(106,73,123), 2);
    } else {
        cv::rectangle(*image, top, low, Scalar(123,121,21), 2);
    }
    (*image).at<Vec3b>(Point(note.x, note.y)) = Vec3b(0, 0, 255);
}

Mat MezzoUtilities::encode_pictoform(list<Note> notes) {
    int size = (int) roundf(sqrtf(notes.size()));
    if(size * size < notes.size()) { size++; }
    Mat pictoform(size * 3 + size, size * 3 + 1, CV_8UC3, cv::Scalar(0, 0, 0));

    for(int i = 0; i < pictoform.rows; i++) {
        pictoform.at<Vec3b>(i, 0)[0] = 0;
        pictoform.at<Vec3b>(i, 0)[1] = 0;
        pictoform.at<Vec3b>(i, 0)[2] = 255;
    }

    for(int i = 0; i < pictoform.rows; i+=4) {
        for(int j = 0; j < pictoform.cols; j++) {
            pictoform.at<Vec3b>(i, j)[0] = 0;
            pictoform.at<Vec3b>(i, j)[1] = 255;
            pictoform.at<Vec3b>(i, j)[2] = 255;
        }
    }

    pictoform.at<Vec3b>(0, 0)[0] = 255;
    pictoform.at<Vec3b>(0, 0)[1] = 255;
    pictoform.at<Vec3b>(0, 0)[2] = 255;

    int i = 1;
    int j = 1;
    for(std::list<Note>::iterator n = notes.begin(); n != notes.end(); n++) {
        //cout << "Note: tone=" << (*n).tone << " id=" << (*n).symbolId << " silence=" << (*n).isSilence << endl;
        int tone = (*n).tone;
        int id = (*n).symbolId;

        // -------------- Tone encoding ----------------- //
        pictoform.at<Vec3b>(i, j)[2] = signbit(tone) ? 255 : 0;
        tone = abs(tone);

        pictoform.at<Vec3b>(i, j+2)[0] = (tone % 2) * 255;
        tone /= 2;
        pictoform.at<Vec3b>(i, j+2)[1] = (tone % 2) * 255;
        tone /= 2;
        pictoform.at<Vec3b>(i, j+2)[2] = (tone % 2) * 255;
        tone /= 2;

        pictoform.at<Vec3b>(i, j+1)[0] = (tone % 2) * 255;
        tone /= 2;
        pictoform.at<Vec3b>(i, j+1)[1] = (tone % 2) * 255;
        tone /= 2;
        pictoform.at<Vec3b>(i, j+1)[2] = (tone % 2) * 255;
        tone /= 2;

        pictoform.at<Vec3b>(i, j)[0] = (tone % 2) * 255;
        tone /= 2;
        pictoform.at<Vec3b>(i, j)[1] = (tone % 2) * 255;
        tone /= 2;
        // ---------------------------------------------- //

        // --------------- ID encoding ------------------ //
        pictoform.at<Vec3b>(i+2, j+2)[0] = (id % 2) * 255;
        id /= 2;
        pictoform.at<Vec3b>(i+2, j+2)[1] = (id % 2) * 255;
        id /= 2;
        pictoform.at<Vec3b>(i+2, j+2)[2] = (id % 2) * 255;
        id /= 2;

        pictoform.at<Vec3b>(i+2, j+1)[0] = (id % 2) * 255;
        id /= 2;
        pictoform.at<Vec3b>(i+2, j+1)[1] = (id % 2) * 255;
        id /= 2;
        pictoform.at<Vec3b>(i+2, j+1)[2] = (id % 2) * 255;
        id /= 2;

        pictoform.at<Vec3b>(i+2, j)[0] = (id % 2) * 255;
        id /= 2;
        pictoform.at<Vec3b>(i+2, j)[1] = (id % 2) * 255;
        id /= 2;
        pictoform.at<Vec3b>(i+2, j)[2] = (id % 2) * 255;
        id /= 2;
        // ---------------------------------------------- //

        // Silence or note encoding
        if((*n).isSilence) {
            pictoform.at<Vec3b>(i+1, j)[0] = 255;
            pictoform.at<Vec3b>(i+1, j)[1] = 255;
            pictoform.at<Vec3b>(i+1, j)[2] = 255;
        } else {
            pictoform.at<Vec3b>(i+1, j)[0] = 0;
            pictoform.at<Vec3b>(i+1, j)[1] = 0;
            pictoform.at<Vec3b>(i+1, j)[2] = 0;
        }

        // Valid note
        pictoform.at<Vec3b>(i+1, j+1)[0] = 255;
        pictoform.at<Vec3b>(i+1, j+1)[1] = 255;
        pictoform.at<Vec3b>(i+1, j+1)[2] = 255;
        
        j += 3;
        if(j >= pictoform.cols) { j = 1; i += 4; }
    }

    return pictoform;
}

list<Note> MezzoUtilities::decode_pictoform(Mat image) {
    list<Note> notes;
    int tone = 0;
    int id = 0;
    bool isSilence = false;
    int j = 0;
    if(image.at<Vec3b>(0, 0)[0] == 255 && image.at<Vec3b>(0, 0)[1] == 255 && image.at<Vec3b>(0, 0)[1] == 255) {
        for(int i = 0; i < image.rows; i++) {
            if( image.at<Vec3b>(i, 0)[1] == 255 ) {
                j = 1;
                //cout << "Reading new pictoform row." << endl;
            } else {
                for(; j < image.cols; j+=3) {
                    //cout << "(" << i << ", " << j << ")" << endl;
                    if((image.at<Vec3b>(i+1, j+1)[0] == 255) && 
                        (image.at<Vec3b>(i+1, j+1)[1] == 255) && 
                        (image.at<Vec3b>(i+1, j+1)[2] == 255)) {
                        
                        //cout << "Decoding new note." << endl; 

                        // -------------- Tone decoding ----------------- //
                        if(image.at<Vec3b>(i, j+2)[0] == 255) tone += (int) pow(2, 0);
                        if(image.at<Vec3b>(i, j+2)[1] == 255) tone += (int) pow(2, 1);
                        if(image.at<Vec3b>(i, j+2)[2] == 255) tone += (int) pow(2, 2);

                        if(image.at<Vec3b>(i, j+1)[0] == 255) tone += (int) pow(2, 3);
                        if(image.at<Vec3b>(i, j+1)[1] == 255) tone += (int) pow(2, 4);
                        if(image.at<Vec3b>(i, j+1)[2] == 255) tone += (int) pow(2, 5);

                        if(image.at<Vec3b>(i, j)[0] == 255) tone += (int) pow(2, 6);
                        if(image.at<Vec3b>(i, j)[1] == 255) tone += (int) pow(2, 7);
                        if(image.at<Vec3b>(i, j)[2] == 255) tone *= -1;
                        // ---------------------------------------------- //

                        // --------------- ID decoding ------------------ //
                        if(image.at<Vec3b>(i+2, j+2)[0] == 255) id += (int) pow(2, 0);
                        if(image.at<Vec3b>(i+2, j+2)[1] == 255) id += (int) pow(2, 1);
                        if(image.at<Vec3b>(i+2, j+2)[2] == 255) id += (int) pow(2, 2);

                        if(image.at<Vec3b>(i+2, j+1)[0] == 255) id += (int) pow(2, 3);
                        if(image.at<Vec3b>(i+2, j+1)[1] == 255) id += (int) pow(2, 4);
                        if(image.at<Vec3b>(i+2, j+1)[2] == 255) id += (int) pow(2, 5);

                        if(image.at<Vec3b>(i+2, j)[0] == 255) id += (int) pow(2, 6);
                        if(image.at<Vec3b>(i+2, j)[1] == 255) id += (int) pow(2, 7);
                        if(image.at<Vec3b>(i+2, j)[2] == 255) id += (int) pow(2, 8);
                        // ---------------------------------------------- //

                        // Silence or note decoding
                        isSilence = (image.at<Vec3b>(i+1, j)[0] == 255) && 
                            (image.at<Vec3b>(i+1, j)[1] == 255) && 
                            (image.at<Vec3b>(i+1, j)[2] == 255);

                        Note decodedNote = Note(0, 0, tone, id, isSilence);
                        notes.push_back(decodedNote);
                        tone = 0;
                        id = 0;
                    }
                }
                i += 2;
            }
        }
    }
    return notes;
}
