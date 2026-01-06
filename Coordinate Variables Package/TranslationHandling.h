/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#ifndef TRANSLATIONHANDLING_H
#define TRANSLATIONHANDLING_H

#include "../Utility Files/Utilities.h"

class CoordinateVariables;
class CoordinateInterpretation;
class LengthAndSpacing;

/**
 * Class for handling the translation of the generated function to the inputted startCoords.
 * Also rotates functions to the correct quadrant
 * And translates endCoords/startCoords to ensure correct function generation.
 */
class TranslationHandling {
private:
    CoordinateVariables* CV;
    CoordinateInterpretation* CI;
    LengthAndSpacing* LS;
public:
    /**
     * Constructor
     * @param CV a pointer for the associated CoordinateVariables object
     */
    TranslationHandling(CoordinateVariables* CV);

    /**
     * Sets objects pointers for the associated classes
     * Modifies: CI, LS
     */
    void setptrs();

    /**
     * Translates x and y coordinates to the correct quadrant
     * Requires: CI->endPointQuadraticCoordinateGenerator(), quadrant, rotationalTranslation, positionalTranslation
     * @param x : generated x value
     * @return : a Coordinates structure containing the corrected x and y coordinates
     */
    Utilities::Coordinates translateArc(double x) const;

    /**
     * Each arc is generated at (0, 0), so this shifts them to the last generated point
     * Requires: startCoords
     * @param coords : a Coordinates structure representing a single set of coordinates in the arc
     * @return : a repositioned "coords"
     */
    Utilities::Coordinates positionalTranslation(Utilities::Coordinates coords) const;
};

#endif //TRANSLATIONHANDLING_H
