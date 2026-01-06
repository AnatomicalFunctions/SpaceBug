/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#ifndef COORDINATEINTERPRETATION_H
#define COORDINATEINTERPRETATION_H

using namespace std;

class CoordinateVariables;
class LengthAndSpacing;
class TranslationHandling;

/**
 * Class that handles initial coordinate processing and function generation
 */
class CoordinateInterpretation {
private:
    CoordinateVariables* CV;
    LengthAndSpacing* LS;
    TranslationHandling* TH;
public:
    /**
     * Constructor
     * @param CV : a pointer for the associated CoordinateVariables object
     */
    CoordinateInterpretation(CoordinateVariables* CV);

    /**
     * Sets object pointers for the associated classes
     * Modifies: LS, TH
     */
    void setptrs();

    /**
     * Generates "a" and compares it to MAX_RATE_OF_CHANGE.
     * If necessary, sets "a" to either positive MAX_RATE_OF_CHANGE or negative depending on whether "a" is positive or negative
     * Requires: a, MAX_RATE_OF_CHANGE, endPointQuadraticFunctionGenerator(), exceedMaxRoC()
     * Modifies: a
     */
    void evaluateA();

    /**
     * Function to calculate f(x) = ax^2 + bx
     * (c is not returned in this function)
     * Requires: a, slope
     * @param x : a given x coordinate
     * @return : f(x)
     */
    double endPointQuadraticCoordinateGenerator(double x) const;

    /**
     * Calculates a
     * Requires: slope, endCoords.x, endCoords.y
     * Modifies: a
     */
    void endPointQuadraticFunctionGenerator();

    /**
     * Function to calculate f(x)' = 2ax + b
     * Requires: slope
     * @param a : the coefficient of x^2 in a quadratic polynomial
     * @param x : a given x coordinate
     * @return : f(x)'
     */
    double fOfXFirstDerivative(double a, double x) const;

    /**
     * Function to calculate f(x)" = 2a
     * @param a : the coefficient of x^2 in a quadratic polynomial
     * @return : f(x)"
     */
    static double fOfXSecondDerivative(double a);

    /**
     * Compares "a" to the set maximum rate of change
     * Requires: a, MAX_RATE_OF_CHANGE
     * @return true if exceeding, false otherwise
     */
    bool exceedMaxRoC() const;
};

#endif //COORDINATEINTERPRETATION_H
