/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#ifndef COORDINATEVARIABLES_H
#define COORDINATEVARIABLES_H

#include <vector>
#include "../Utility Files/Utilities.h"
#include "CoordinateInterpretation.h"
#include "LengthAndSpacing.h"
#include "TranslationHandling.h"
using namespace std;

/**
 * Abstract math class representing variables shared between several component classes, connected with pointers.
 * Handles travel across different arcs in 2D space.
 */
class CoordinateVariables {
private:
    Utilities::Coordinates startCoords;
    Utilities::Coordinates endCoords; //represents the target position translated into the RIGHT quadrant
    Utilities::Coordinates actualEndCoords; //represents the target position without being translated
    vector<Utilities::Coordinates> vCoords;
    double slope;
    double a; //variable representing the coefficient of x^2 in a quadratic polynomial: f(x) = 2ax^2 + bx + c
    double length; //variable representing the calculated length of the current arc of the function
    Utilities::Quadrants quadrant;
    int interpolationIncrement{0}; //indexes within vCoords
public:
    CoordinateInterpretation CI;
    LengthAndSpacing LS;
    TranslationHandling TH;
    //This technically represents the maximum rate of change divided by two, as per
    //f(x)" = 2a, therefore a = MAX / 2
    //Instead, I have subbed the expression of MAX / 2 for this single variable.
    //This may become a problem if we begin utilizing cubic polynomials (f(x)" = 6dx + 2a)
    //without changing the logic by which "a" is reset in CoordinateInterpretation::evaluateA()
    double MAX_RATE_OF_CHANGE;
    //The maximum length between generated coordinates in each arc. Arc length segment length.
    double MAX_SEGMENT_LENGTH;
    //Used as a determinant for how close the accepted value is to the true value.
    static constexpr double PRECISION_CONSTANT{0.0001};
    //The maximum allowed slope before switching quadrants. Not easily variable.
    //Note that slopes above 1 have a drop-off as they approach infinity.
    static constexpr double MAX_SLOPE{0.5};
    //Circularization constant... changes the way slope is altered between arc generations.
    //I experimented with making this a scale based on "a", so MAXRoC * CONST = 0.2, and then scaled for other "a" values,
    //But it looked more jagged than when it's just a constant 0.2
    static constexpr double CIRCULARIZATION_CONSTANT{0.2};

    /**
     * Constructor
     * @param startCoords : variable representing x and y coordinates as well as slope and quadrant
     * @param endCoords : variable representing x and y coordinates for the end set
     */
    CoordinateVariables(
        Utilities::Coordinates startCoords,
        Utilities::Coordinates endCoords,
        double MAX_RATE_OF_CHANGE,
        double MAX_SEGMENT_LENGTH
    );

    /**
     * Same as the regular constructor but with items passed in by a struct.
     */
    CoordinateVariables(Utilities::CVBundle cv_bundle);

    /**
     * Constructor for the purpose of SpaceCraft objects.
     * Does not call the initialization function of CoordinateVariables
     * Does not take endCoords.
     */
    CoordinateVariables(
        Utilities::Coordinates startCoords,
        double MAX_RATE_OF_CHANGE,
        double MAX_SEGMENT_LENGTH
    );

    /**
     * Copy constructor implemented for use in Utilities::goForChaff()
     */
    CoordinateVariables(const CoordinateVariables& other);

    /**
     * Empty overloaded function
     */
    virtual void initialization();

    /**
     * Iterates through vCoords and calls arcGenCall() when the end is reached.
     * Requires: arcGenCall(), interpolation(), interpolationIncrement
     * Modifies: interpolationIncrement
     * @return : an object at index interpolationIncrement of vCoords
     */
    Utilities::Coordinates interpolation();

    /**
     * Specifies how to populate vCoords
     * Requires: singlePointGen();
     */
    virtual void arcGenCall();

    /**
     * Generates a single point on an arc up until the quadrant boundary or until it hits the target.
     * Very similar to arcGen, but reconfigured to call singleCoordNumericAnalysis() and validate when to change quadrants
     * Requires: CI.evaluateA(), LS.slopeBasedEndPoint(), targetInQuadrant(), LS.singleCoordNumericAnalysis(),
     *           LS.slopeGen(), MAX_SLOPE, CIRCULARIZATION_CONSTANT, MAX_SEGMENT_LENGTH, vCoords, actualEndCoords
     * Modifies: startCoords, endCoords, slope, quadrant, length
     */
    void singlePointGen();

    /**
     * Checks if the target is within the current quadrant and alters vCoords accordingly
     * Mostly here to centralize the same operation between singlePointGen() and arcGen()
     * Requires: CI.endPointQuadraticCoordinateGenerator(), LS.arcLengthCalculator(), LS.singleCoordNumericAnalysis(),
     *           LS.slopeGen(), PRECISION_CONSTANT, vCoords, endCoords, actualEndCoords, a, slope, quadrant
     * Modifies: startCoords, vCoords, slope, length
     * @param length_to_bound : Arc length up until x crosses MAX_SLOPE
     * @return : true if the target is within the current quadrant, false otherwise.
     */
    bool targetInQuadrant(double length_to_bound);

    /**
     * Rate of change-based arc generation, recursive.
     * Calls numericAnalysis based on different lengths. Handles quadrant changes if necessary.
     * Requires: MAX_SLOPE, CIRCULARIZATION_CONSTANT, LS.numericAnalysis(),
     *           LS.slopeBasedEndPoint(), LS.slopeGen(), TH.toRightTranslation(), rocArcGenRecursion()
     * Modifies: length, slope, startCoords, quadrant
     * @param length : specified length of requested arc. Changes when called recursively.
     */
    void rocArcGenRecursion(double length);

    /**
     * Cleaner function for vCoords and resets necessary variables for consistent generation.
     * Relies on interpolation increments for reset, so doesn't work when accessed during pure arcGen runs.
     * Requires: vCoords, startCoords, actualEndCoords, interpolationIncrement, CV/SC->arcGenCall()
     * Modifies: vCoords, startCoords, endCoords, slope, quadrant, interpolationIncrement
     */
    virtual void resetVCoords();

    /**
     * Resets the object for all new information.
     * Requires: CV/SC->intialization()
     * Modifies: startCoords, endCoords, actualEndCoords, slope, quadrant, vCoords, interpolationIncrement,
     *           interpolationSegmentIncrement, interpolationStartCoords
     * See CoordinateVariables() constructor for param information.
     */
    virtual void reset(
        Utilities::Coordinates startCoords,
        Utilities::Coordinates endCoords
    );

    /**
     * Resets the target, represented primarily by actualEndCoords.
     * Requires: setVCoordsClearToIndex(), singlePointGen()
     * Modifies: vCoords, startCoords, endCoords, actualEndCoords, quadrant, slope
     * @param endCoords : an updated target
     */
    void newTargetPosition(Utilities::Coordinates endCoords);

    //Getters
    Utilities::Coordinates getStartCoords() const;
    Utilities::Coordinates getEndCoords() const;
    Utilities::Coordinates getActualEndCoords() const;
    vector<Utilities::Coordinates> getVCoords() const;
    double getSlope() const;
    double getA() const;
    double getLength() const;
    Utilities::Quadrants getQuadrant() const;
    int getInterpolationIncrement() const;

    //Setters
    void setStartCoords(Utilities::Coordinates startCoords);
    //Translates incoming endCoords through TH.endCoordsTranslation()
    void setEndCoords(Utilities::Coordinates endCoords);
    void setActualEndCoords(Utilities::Coordinates actualEndCoords);
    void setVCoords(Utilities::Coordinates coords);
    //Clears VCoords past the inputted index
    void setVCoordsClearAfterIndex(int index);
    void setSlope(double slope);
    void setA(double a);
    void setLength(double length);
    void setQuadrant(Utilities::Quadrants quadrant);
    //sets next quadrant based on variable "a"
    void setQuadrant();
    void setInterpolationIncrement(int interpolationIncrement);

    /**
     * Prints certain information within a Coordinate Variables object when called.
     * @param os : the desired outstream
     */
    void coordinatePrinter(ostream &os) const;

    /**
     * Overloads the << operator to print the information for a given CoordinateVariables object
     * @param os : the desired outstream
     * @param CV : a CoordinateVariables object
     * @return : the desired outstream, returned for continuation
     */
    friend ostream& operator<<(ostream& os, const CoordinateVariables& CV);
};

#endif //COORDINATEVARIABLES_H

