/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#ifndef LENGTHANDSPACING_H
#define LENGTHANDSPACING_H

class CoordinateVariables;
class CoordinateInterpretation;
class TranslationHandling;

/**
 * Class containing the functions to calculate arc length
 * And generate equally spaced coordinates across a given arc
 * Also handles searching for specific points based on slope
 * Also handles interpolation
 */
class LengthAndSpacing {
private:
    CoordinateVariables* CV;
    CoordinateInterpretation* CI;
    TranslationHandling* TH;
public:
    /**
     * Constructor
     * @param CV : a pointer for the associated CoordinateVariables object
     */
    LengthAndSpacing(CoordinateVariables* CV);

    /**
     * Sets objects pointers for the associated classes
     * Modifies: CI, TH
     */
    void setptrs();

    /**
     * Evaluates the arc length
     * Requires: a, slope, CI->fOfXFirstDerivative(), integratedArcLength()
     * @param x : endCoords.x
     * @return : arc length up to a given x, over a specific slope
     */
    double arcLengthCalculator(double x) const;

    /**
     * The indefinite integral of the Arc Length Formula
     * @param x : a given x coordinate, representing one end of an interval
     * @return : length up to given x
     */
    static double integratedArcLength(double x);

    /**
     * Determines the boundary used by slopeBasedRecursion as well as other initial variables.
     * Requires: slopeBasedRecursion(), arcLengthCalculator(), MAX_SLOPE
     * @return : the length of the arc up to MAX_SLOPE, or 0, or arcLengthCalculator(CV->getEndCoords().x)
     */
    double slopeBasedEndPoint() const;

    /**
     * Generates an x coordinate matching a specified slope within an arc.
     * Requires: slopeGen(), MAX_SLOPE, slopeBasedRecursion()
     * @param i : the degree to which x is made smaller during the function's search
     * @param x : unknown value of x able to generate the target slope
     * @param bound : the maximum or minimum slope value
     * @return : x
     */
    double slopeBasedRecursion(int i, const double x, const double bound) const;

    /**
     * Finds the slope tangent to the curve at a specific coordinate
     * Requires: CI->endPointQuadraticCoordinateGenerator(), PRECISION_CONSTANT
     * @param x : a given x coordinate
     * @return : the slope tangent to the curve
     */
    double slopeGen(double x) const;

    /**
     * Fills a vector with equally spaced coordinates that traverse a given arc.
     * And generates the number of coordinates needed to traverse an arc based on MAX_SEGMENT_LENGTH
     * Deviates from MAX_SEG slightly to fill the entire arc and spreads the deviation across the generated coordinates
     * Requires: MAX_SEGMENT_LENGTH, length, TH->translateArc(), numericRecursion()
     * Modifies: coords
     */
    void numericAnalysis();

    /**
     * Fills a vector with a single coordinate to traverse a given arc.
     * Deviates from MAX_SEGMENT_LENGTH slightly to fill the entire arc,
     * and spreads the deviation across the generated coordinates in successive calls
     * Requires: MAX_SEGMENT_LENGTH, length, TH->translateArc(), numericRecursion()
     * Modifies: coords
     */
    void singleCoordNumericAnalysis();

    /**
     * Generates an x coordinate matching a specified length across an arc.
     * Requires: PRECISION_CONSTANT, arcLengthCalculator(), numericRecursion()
     * @param i : the degree to which x is made smaller during the function's search
     * @param x : unknown value of x able to generate the target length
     * @param target_length : length of the arc segment
     * @return : x
     */
    double numericRecursion(int i, double x, const double target_length) const;
};

#endif //LENGTHANDSPACING_H
