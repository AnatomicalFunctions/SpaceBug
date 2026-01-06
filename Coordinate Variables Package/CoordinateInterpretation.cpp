/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#include <cmath>
#include "CoordinateVariables.h"
#include "../Utility Files/Utilities.h"
#include "CoordinateInterpretation.h"
#include "LengthAndSpacing.h"
#include "TranslationHandling.h"

using namespace std;

CoordinateInterpretation::CoordinateInterpretation(CoordinateVariables* CV) : CV(CV) {}

void CoordinateInterpretation::setptrs() {
    LS = &CV->LS;
    TH = &CV->TH;
}

void CoordinateInterpretation::evaluateA() {
    endPointQuadraticFunctionGenerator();
    //The program will erroneously generate a function that intersects a point behind (0, 0)
    //This makes it so any end point seen as behind (0, 0) will switch "a" to MAXRoC
    //Instead of creating a backward intercept and breaking the program.
    if (CV->getEndCoords().x < 0) {
        if (CV->getEndCoords().y < 0) {
            CV->setA(-CV->MAX_RATE_OF_CHANGE);
        } else { //end points at y = 0 default to positive "a"
            CV->setA(CV->MAX_RATE_OF_CHANGE);
        }
    } else if (exceedMaxRoC()) { //regular MAXRoC enforcement
        if (CV->getA() < 0) {
            CV->setA((-CV->MAX_RATE_OF_CHANGE)); //accounts for negative a
        } else {
            CV->setA(CV->MAX_RATE_OF_CHANGE);
        }
    //skirts a nan value returned by division by zero in the endPointQuadraticFunctionGenerator()
    } else if (CV->getEndCoords().x == 0) {
        CV->setA(CV->MAX_RATE_OF_CHANGE);
    }
}

/**
 * Quadratic Polynomial:
 * f(x) = ax^2 + bx + c
 * f(x)' = 2ax + b
 * f(x)" = 2a
 *
 * a = ((endCoords.y - (slope * endCoords.x)) / pow(endCoords.x, 2))
 * b = ((endCoords.y - (((endCoords.y - (slope * endCoords.x)) / pow(endCoords.x, 2)) * pow(endCoords.x, 2))) / endCoords.x)
 * c = (startCoords.y)
 *
 * b simplifies to slope
 *
 * I eliminated startCoords x and y by setting them to a constant zero, so neither appear in this equation.
 * This is accounted for in TH->positionalTranslationHandling()
 */
double CoordinateInterpretation::endPointQuadraticCoordinateGenerator(double x) const {
    //Quadratic polynomial f(x) = ax^2 + bx + c
    //Where:
    //a = ((endCoords.y - (slope * endCoords.x)) / pow(endCoords.x, 2))
    //b = (slope)
    return ((CV->getA() * pow(x, 2)) + //ax^2
            (CV->getSlope() * x)); //bx
}

void CoordinateInterpretation::endPointQuadraticFunctionGenerator() {
    CV->setA((CV->getEndCoords().y - (CV->getSlope() * CV->getEndCoords().x)) / pow(CV->getEndCoords().x, 2));
}

double CoordinateInterpretation::fOfXFirstDerivative(double a, double x) const {
    //f(x)' = 2ax + b
    return (2 * a * x) +
           (CV->getSlope());
}

double CoordinateInterpretation::fOfXSecondDerivative(double a) {
    //f(x)" = 2a
    return (2 * a);
}

//fOfXSecondDerivative(endPointQuadraticFunctionGenerator()) > MAX_RATE_OF_CHANGE
bool CoordinateInterpretation::exceedMaxRoC() const {
    return (abs(CV->getA()) > CV->MAX_RATE_OF_CHANGE);
}
