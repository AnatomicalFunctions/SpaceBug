/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#include "CoordinateVariables.h"
#include "CoordinateInterpretation.h"
#include "LengthAndSpacing.h"
#include "TranslationHandling.h"
#include <cmath>
#include <iostream>
using namespace std;

LengthAndSpacing::LengthAndSpacing(CoordinateVariables* CV) : CV(CV) {
}

void LengthAndSpacing::setptrs() {
    this->CI = &CV->CI;
    this->TH = &CV->TH;
}

/**
 * Additional information:
 *
 * This function can only integrate from 0
 *
 * u-substitution:
 * u = 2ax + b
 * du = 2a * dx
 * dx = du / 2a
 * This is why the equation includes the division "(2 * a)", though the equation simplifies, and it becomes (4 * a)
 *
 * Because startCoords.x isn't used, and its placeholder will always be 0, the theoretical startX simplifies to slope.
 *
 * I just found out that cmath log() actually represents ln... based on e instead of 10
 */
double LengthAndSpacing::arcLengthCalculator(double x) const {
    //If a == 0, the equation is a straight line, and must be evaluated slightly differently.
    if ((CV->getA() + CV->PRECISION_CONSTANT) > 0 &&
        (CV->getA() - CV->PRECISION_CONSTANT) < 0) {
        /**
         * f(x)' = 2ax + b simplifies to b when a == 0,
         * so the equation becomes a constant and can be simplified to:
         * Arc Length Formula * (endCoords.x - 0) (this is the interval x = 0 to x = endCoords.x)
         * I could also have used the distance formula here:
         * sqrt(pow((endCoords.x - startCoords.x), 2) + pow((endCoords.y - startCoords.y), 2))
         */
        return ((sqrt(1 + pow(CV->getSlope(), 2)) * x));
    }
    // Integral over interval x = 0 to x = endCoords.x
    // sqrt(1 + pow(f(x)', 2)) * dx (this is the Arc Length Formula)
    return (((integratedArcLength(CI->fOfXFirstDerivative(CV->getA(), x))) -
             (integratedArcLength(CV->getSlope()))) /
             (4 * CV->getA())); //du correction
}

double LengthAndSpacing::integratedArcLength(double x) {
    //Integrated arc length formula
    return ((x * sqrt(1 + pow(x, 2))) +
             log(x + sqrt(1 + pow(x, 2))));
}

double LengthAndSpacing::slopeBasedEndPoint() const {
    //when a == 0, there is no slopeBasedEndPoint
    if (CV->getA() == 0) { //sets length_to_bound to one more than MAX_SEGMENT_LENGTH in CV
        return CV->MAX_SEGMENT_LENGTH + 1;
    }
    //If the place we're starting from already exceeds the bound,
    //This tells arcGen to change quadrants and try again, in a roundabout way
    if ((CV->getSlope() < -CV->MAX_SLOPE && CV->getA() < 0) ||
        (CV->getSlope() > CV->MAX_SLOPE && CV->getA() > 0)) {
        return 0;
    }
    double bound = CV->MAX_SLOPE;
    //if "a" is less than 0, the slope is expected to cross -MAX_SLOPE
    if (CV->getA() < 0) {bound = -CV->MAX_SLOPE;}
    //I set x to 1 for no real reason... it quickly shifts accordingly.
    return (arcLengthCalculator(slopeBasedRecursion(-1, 1, bound)));
}

double LengthAndSpacing::slopeBasedRecursion(int i, double x, const double bound) const {
    //checks if the test slope is within a precise range of the target slope
    if ((bound + CV->PRECISION_CONSTANT) > (slopeGen(x)) &&
        (bound - CV->PRECISION_CONSTANT) < (slopeGen(x))) {
        return x;
    }
    //x is made larger until its slope exceeds the slope boundary
    //this will only be performed at the first call to set initial x
    while (bound == CV->MAX_SLOPE ? //false when the bound is negative MAX_SLOPE
           bound > slopeGen(x) :
           bound < slopeGen(x)) {
        x *= 2;
    }
    //x is bisected and then made larger on each pass in smaller increments
    while (bound == CV->MAX_SLOPE ?
           bound > (slopeGen(x * (1 - pow(2, i)))) :
           bound < (slopeGen(x * (1 - pow(2, i))))) {
        i -= 1;
        if (i < -20) {return x;} //prevents infinite recursion. Error occurs maybe 1 time out of 2000. Not sure why.
    }
    //sets x to the lowest number found to be greater than the target slope
    return slopeBasedRecursion(i, (x * (1 - pow(2, i))), bound);
}

double LengthAndSpacing::slopeGen(double x) const {
    //(y2 - y1) / (x2 - x1)
    //using the precision constant here isn't entirely necessary, but it works well.
    return ((CI->endPointQuadraticCoordinateGenerator(x + CV->PRECISION_CONSTANT) -
             CI->endPointQuadraticCoordinateGenerator(x)) / CV->PRECISION_CONSTANT);
}

void LengthAndSpacing::numericAnalysis() {
    //n is the total amount of numbers in the set to be found.
    //Jumps between arcs may be caused by this method, but interpolation should make movement constant.
    int n = CV->getLength() / CV->MAX_SEGMENT_LENGTH;
    for (int i = 1; i <= n; i++) {
        double end_of_arc_segment = (CV->MAX_SEGMENT_LENGTH * i);
        //Generate, translate, and push Coordinates structures into the Coords vector
        CV->setVCoords(TH->translateArc(numericRecursion(-1, end_of_arc_segment, end_of_arc_segment)));
    }
}

void LengthAndSpacing::singleCoordNumericAnalysis() {
    //Generate, translate, and push Coordinates structures into the Coords vector
    CV->setVCoords(TH->translateArc(numericRecursion(-1, CV->MAX_SEGMENT_LENGTH, CV->MAX_SEGMENT_LENGTH)));
}

/**
 * Additional Information:
 *
 * numericRecursion calls the arcLengthCalculator to integrate over the period of x = 0 to x = ?
 * In an effort to generate an x coordinate that matches a specific produced length.
 *
 * x begins by matching the target_length,
 * but x is always larger or equal, as the arc will always produce a shorter or equal x-traversal over the same length.
 */
double LengthAndSpacing::numericRecursion(int i, const double x, const double target_length) const {
    double test_length = arcLengthCalculator(x);
    //checks if the test_length is within a precise range of the target_length
    if ((target_length + CV->PRECISION_CONSTANT) > test_length &&
        (target_length - CV->PRECISION_CONSTANT) < test_length) {
        return x;
    }
    //x is bisected and then made larger on each pass in smaller increments
    while (target_length > arcLengthCalculator((x * (1 - pow(2, i))))) {
        i -= 1;
        if (i < -20) {return x;} //prevents infinite recursion. Error occurs maybe 1 time out of 2000. Not sure why.
    }
    //sets x to the lowest number found to be greater than the target_length
    return numericRecursion(i, (x * (1 - pow(2, i))), target_length);
}