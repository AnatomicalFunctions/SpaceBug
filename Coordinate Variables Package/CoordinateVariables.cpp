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
#include "../Utility Files/StaticTranslationHandling.h"
#include "../Utility Files/Utilities.h"
#include <iomanip>
#include <format>
#include <iostream>
#include <cmath>
using namespace std;

CoordinateVariables::CoordinateVariables(
    Utilities::Coordinates startCoords,
    Utilities::Coordinates endCoords,
    double MAX_RATE_OF_CHANGE,
    double MAX_SEGMENT_LENGTH
    ) :
    startCoords(startCoords),
    //endCoords must be translated through setEndCoords(), so endCoords(endCoords) does not appear here.
    actualEndCoords(endCoords),
    slope(startCoords.slope),
    quadrant(startCoords.quadrant),
    MAX_RATE_OF_CHANGE(MAX_RATE_OF_CHANGE),
    MAX_SEGMENT_LENGTH(MAX_SEGMENT_LENGTH),
    CI(this),
    LS(this),
    TH(this
    ) {
    setEndCoords(endCoords);
    this->CI.setptrs(); //These do not need to be initialized in any particular order
    this->LS.setptrs();
    this->TH.setptrs();
}

CoordinateVariables::CoordinateVariables(
    Utilities::CVBundle cv_bundle
    ) :
    startCoords(cv_bundle.startCoords),
    //endCoords must be translated through setEndCoords(), so endCoords(endCoords) does not appear here.
    actualEndCoords(cv_bundle.endCoords),
    slope(cv_bundle.startCoords.slope),
    quadrant(cv_bundle.startCoords.quadrant),
    MAX_RATE_OF_CHANGE(cv_bundle.MAX_RATE_OF_CHANGE),
    MAX_SEGMENT_LENGTH(cv_bundle.MAX_SEGMENT_LENGTH),
    CI(this),
    LS(this),
    TH(this
    ) {
    setEndCoords(cv_bundle.endCoords);
    this->CI.setptrs(); //These do not need to be initialized in any particular order
    this->LS.setptrs();
    this->TH.setptrs();
}

CoordinateVariables::CoordinateVariables(
    Utilities::Coordinates startCoords,
    double MAX_RATE_OF_CHANGE,
    double MAX_SEGMENT_LENGTH
    ) :
    startCoords(startCoords),
    slope(startCoords.slope),
    quadrant(startCoords.quadrant),
    MAX_RATE_OF_CHANGE(MAX_RATE_OF_CHANGE),
    MAX_SEGMENT_LENGTH(MAX_SEGMENT_LENGTH),
    CI(this),
    LS(this),
    TH(this
    ) {
    this->CI.setptrs(); //These do not need to be initialized in any particular order
    this->LS.setptrs();
    this->TH.setptrs();
}

CoordinateVariables::CoordinateVariables(
    const CoordinateVariables& other
    ) :
    startCoords(other.startCoords),
    endCoords(other.endCoords),
    actualEndCoords(other.actualEndCoords),
    vCoords(other.vCoords),
    slope(other.slope),
    a(other.a),
    length(other.length),
    quadrant(other.quadrant),
    interpolationIncrement(other.interpolationIncrement),
    MAX_RATE_OF_CHANGE(other.MAX_RATE_OF_CHANGE),
    MAX_SEGMENT_LENGTH(other.MAX_SEGMENT_LENGTH),
    CI(this),
    LS(this),
    TH(this
    ) {
    CI.setptrs();
    LS.setptrs();
    TH.setptrs();
}

void CoordinateVariables::initialization() {
    cout << "CV Initialization Call Not Allowed" << endl;
    exit(EXIT_FAILURE); // -------------------------------------------------------------------------- EXIT PROGRAM
}

Utilities::Coordinates CoordinateVariables::interpolation() {
    try {
        Utilities::Coordinates returnCoords = getVCoords().at(getInterpolationIncrement());
        setInterpolationIncrement(getInterpolationIncrement() + 1);
        return returnCoords;
    } catch (...) {
        arcGenCall();
        return interpolation();
    }
}

void CoordinateVariables::arcGenCall() {
    singlePointGen();
}

void CoordinateVariables::singlePointGen() {
    //checks if the function will hit the target before the slope hits the boundary
    //also accounts for straight-line functions
    double length_to_bound = LS.slopeBasedEndPoint();
    if (!targetInQuadrant(length_to_bound)) { //if false, run arc until the quadrant boundary is hit
        //Since we're generating single coords, we aren't always hitting the quadrant boundary.
        //This checks if we are, and makes changes accordingly.
        //If true, either LS.singleCoordNumericAnalysis won't be able to generate anything,
        //or it's close enough to the boundary that we can safely change quadrants.
        if (length_to_bound < MAX_SEGMENT_LENGTH) {

            double circular_modifier{(2 * MAX_SLOPE) - CIRCULARIZATION_CONSTANT};
            if (getA() > 0) {circular_modifier *= -1;}
            setSlope(getStartCoords().slope + circular_modifier);

            setQuadrant(); //sets for next run based on "a"
            setEndCoords(getActualEndCoords()); //resets for next translation after new quadrant is known
            CI.evaluateA();
            singlePointGen(); //Run with new quadrant in order to generate coordinates.
            return;
        }
        setLength(length_to_bound);
        LS.singleCoordNumericAnalysis();
    }
    setStartCoords(getVCoords().back());
    setSlope(getStartCoords().slope);
    setEndCoords(getActualEndCoords()); //resets for next translation after new quadrant is known
}

bool CoordinateVariables::targetInQuadrant(double length_to_bound) {
    double length_to_target = LS.arcLengthCalculator(getEndCoords().x);
    double test_y = CI.endPointQuadraticCoordinateGenerator(getEndCoords().x);
    if (test_y > getEndCoords().y - PRECISION_CONSTANT &&
        test_y < getEndCoords().y + PRECISION_CONSTANT &&
        length_to_target <= length_to_bound) {
        if (length_to_target > 0) {
            setLength(length_to_target);
            LS.singleCoordNumericAnalysis();
            return true;
        }
    }
    //If length was length_to_target in the last run, and the missile went through the target,
    //The current length_to_target will be less than length.
    //If the last length was set to length_to_bound, the current length_to_bound will be less than length.
    //These factors combine to fire evaluateA() right after the missile has gone through the target, locking-in an
    //"a" value without mixing it up in successive evaluations.
    //WARNING: There is undefined behavior going on here, and it sometimes fires unnecessarily, but these random calls
    //Don't currently affect behavior, and it also runs when it needs to, so I'm leaving it as-is.
    if (getLength() > length_to_target &&
        getLength() < length_to_bound) {
        CI.evaluateA();
    }
    return false;
}

//Notice that endCoords is NOT utilized... it plays no role in length-based movement.
void CoordinateVariables::rocArcGenRecursion(double length) {
    //if a == 0, this returns a junk value.
    double length_to_bound = LS.slopeBasedEndPoint();
    //if we hit the boundary before finishing out the requested length.
    if (length_to_bound <= length && getA() != 0) {
        //This is the code used to run the arc until the quadrant boundary is hit.
        if (length_to_bound > MAX_SEGMENT_LENGTH) { //skips if numericAnalysis isn't going to add anything
            setLength(length_to_bound);
            LS.numericAnalysis();
            setStartCoords(getVCoords().back());
        }
        double circular_modifier{(2 * MAX_SLOPE) - CIRCULARIZATION_CONSTANT};
        if (getA() > 0) {circular_modifier *= -1;}
        setSlope(getStartCoords().slope + circular_modifier);
        setQuadrant();
        //Run again to finish out the remainder of the requested length.
        rocArcGenRecursion(length - length_to_bound);
    } else {
        setLength(length);
        if (int(getLength() / MAX_SEGMENT_LENGTH) != 0) { //if numericAnalysis is going to add anything
            LS.numericAnalysis();
            setStartCoords(getVCoords().back());
            setSlope(getStartCoords().slope);
        }
    }
}

void CoordinateVariables::resetVCoords() {
    setStartCoords(getVCoords().at(getInterpolationIncrement()));
    vCoords.clear();
    setVCoords(getStartCoords());
    setQuadrant(getStartCoords().quadrant);
    setSlope(getStartCoords().slope);
    setEndCoords(getActualEndCoords());
    setInterpolationIncrement(0);
    arcGenCall();
}

void CoordinateVariables::reset(
    Utilities::Coordinates startCoords,
    Utilities::Coordinates endCoords
    ) {
    setStartCoords(startCoords);
    vCoords.clear();
    setQuadrant(startCoords.quadrant);
    setSlope(startCoords.slope);
    setEndCoords(endCoords);
    setActualEndCoords(endCoords);
    setInterpolationIncrement(0);
    initialization();
}

void CoordinateVariables::newTargetPosition(Utilities::Coordinates endCoords) {
    //Erases unused arc-builder data
    setVCoordsClearAfterIndex(getInterpolationIncrement());
    setStartCoords(getVCoords().back());
    setQuadrant(getStartCoords().quadrant);
    setSlope(getStartCoords().slope);
    setEndCoords(endCoords);
    setActualEndCoords(endCoords);
    if (abs(getA()) != MAX_RATE_OF_CHANGE) { //Prevents over-evaluation that leads to oscillating movement.
        CI.evaluateA();
    }
    singlePointGen();
}

//Getters
Utilities::Coordinates CoordinateVariables::getStartCoords() const {
    return startCoords;
}

Utilities::Coordinates CoordinateVariables::getEndCoords() const {
    return endCoords;
}

Utilities::Coordinates CoordinateVariables::getActualEndCoords() const {
    return actualEndCoords;
}

vector<Utilities::Coordinates> CoordinateVariables::getVCoords() const {
    return vCoords;
}


double CoordinateVariables::getSlope() const {
    return slope;
}

double CoordinateVariables::getA() const {
    return a;
}

double CoordinateVariables::getLength() const {
    return length;
}

Utilities::Quadrants CoordinateVariables::getQuadrant() const {
    return quadrant;
}

int CoordinateVariables::getInterpolationIncrement() const {
    return interpolationIncrement;
}

//Setters
void CoordinateVariables::setStartCoords(Utilities::Coordinates startCoords) {
    this->startCoords = startCoords;
}

void CoordinateVariables::setEndCoords(Utilities::Coordinates endCoords) {
    endCoords = Utilities::Coordinates{endCoords.x, endCoords.y, 0, 0, getQuadrant()};
    this->endCoords = StaticTranslationHandling::toRightTranslation(endCoords, getStartCoords());
}

void CoordinateVariables::setActualEndCoords(Utilities::Coordinates actualEndCoords) {
    this->actualEndCoords = actualEndCoords;
}


void CoordinateVariables::setVCoords(Utilities::Coordinates coords) {
    this->vCoords.push_back(coords);
}

void CoordinateVariables::setVCoordsClearAfterIndex(int index) {
    int size = (vCoords.size() - index - 1); //prevents altering size mid-loop
    for (int i = 0; i < size; i++) {
        vCoords.pop_back();
    }
}

void CoordinateVariables::setSlope(double slope) {
    this->slope = slope;
}

void CoordinateVariables::setA(double a) {
    this->a = a;
}

void CoordinateVariables::setLength(double length) {
    this->length = length;
}

void CoordinateVariables::setQuadrant(Utilities::Quadrants quadrant) {
    this->quadrant = quadrant;
}

void CoordinateVariables::setQuadrant() {
    if (getA() < 0) {
        if (quadrant == Utilities::UPRIGHT) {
            quadrant = Utilities::RIGHT;
        } else {
            quadrant = Utilities::Quadrants(int(quadrant) + 1);
        }
    } else if (getA() > 0) {
        if (quadrant == Utilities::RIGHT) {
            quadrant = Utilities::UPRIGHT;
        } else {
            quadrant = Utilities::Quadrants(int(quadrant) - 1);
        }
    } //if a == 0, change nothing
}

void CoordinateVariables::setInterpolationIncrement(int interpolationIncrement) {
    this->interpolationIncrement = interpolationIncrement;
}

void CoordinateVariables::coordinatePrinter(ostream &os) const {
    os << getStartCoords() << endl;
    os << getEndCoords() << endl;
    os << "Arc length: " << format("{:.3f}", getLength()) << endl;
    os << "ax^2 + bx + c:" << endl;
    os << getA() << "x^2 + " << getSlope() << "x" << endl;
}

ostream& operator<<(ostream& os, const CoordinateVariables& CV) {
    CV.coordinatePrinter(os);
    return os;
}