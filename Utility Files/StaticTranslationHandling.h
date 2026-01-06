/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#ifndef STATICTRANSLATIONHANDLING_H
#define STATICTRANSLATIONHANDLING_H

#include "Utilities.h"

/**
 * Namespace holding the static translation functions so they can be used for rotation outside of the CV objects.
 */
namespace StaticTranslationHandling {
    /**
    * Each arc is generated in the RIGHT quadrant, so this rotates them to the correct quadrant
    * (Run before positionalTranslation())
    * Requires: quadrant, ninetyTranslation(), fourFiveTranslation()
    * @param coords : a Coordinates structure representing a single set of coordinates in the arc
    * @return : A rotated "coords"
    */
    Utilities::Coordinates fromRightTranslation(Utilities::Coordinates coords);

    /**
     * To generate the correct arc, endCoords must be rotated from the current quadrant to the RIGHT quadrant
     * And the current startCoords must be subtracted from the actualEndCoords, created a new endCoords
     * Requires: fourFiveBackTranslation(), ninetyTranslation()
     * @param coords : a Coordinates structure representing the target (actualEndCoords)
     * @param pos_coords : a Coordinates structure representing a required positional translation
     * which differs between whether this is being called for endCoords or startCoords
     * @return : a translated endCoords used to generate a new arc
     */
    Utilities::Coordinates toRightTranslation(Utilities::Coordinates coords,
                                                     Utilities::Coordinates pos_coords);

    /**
     * Translates a Coordinates structure 90 degrees a given # of times.
     * @param runs : The number of 90 degree turns required for a specific translation
     * @return : a translated structure
     */
    Utilities::Coordinates ninetyTranslation(Utilities::Coordinates coords, int runs);

    /**
     * @return : A Coordinates structure translated from RIGHT to DOWNRIGHT
     */
    Utilities::Coordinates fourFiveTranslation(Utilities::Coordinates coords);

    /**
     * @return : A Coordinates structure translated from DOWNRIGHT to RIGHT
     */
    Utilities::Coordinates fourFiveBackTranslation(Utilities::Coordinates coords);

    /**
     * @return : A Coordinates structure translated one degree clockwise
     */
    Utilities::Coordinates oneDegClockwiseTranslation(Utilities::Coordinates coords);

    /**
     * @return : A Coordinates structure translated one degree counter-clockwise
     */
    Utilities::Coordinates oneDegCounterTranslation(Utilities::Coordinates coords);
}

#endif //STATICTRANSLATIONHANDLING_H
