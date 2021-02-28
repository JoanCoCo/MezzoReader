/**
 * Staff.cpp
 *  
 * SMII 2020/2021
 * Joan Colom Colom
 *  
 * Implemetation of the class described in Staff.h.
 */

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <list>
#include "../include/Utilities.h"
#include "../include/Staff.h"

using namespace std;
using namespace cv;

Staff::Staff(int *l, int ul, int ll) {
    for(int i = 0; i < 5; i++) {
        lines[i] = l[i];
    }
    spaceBetweenLines = get_space_between_lines();
    upperLimit = std::max(lines[0] - 5 * spaceBetweenLines, ul);
    lowerLimit = std::min(lines[4] + 5 * spaceBetweenLines, ll);
}

int Staff::get_space_between_lines() {
    int spaceBetweenLines = 0;
    for(int i = 0; i < 4; i++) {
        spaceBetweenLines = spaceBetweenLines + (lines[i+1] - lines[i]);
    }
    spaceBetweenLines /= 4;
    return spaceBetweenLines;
}
