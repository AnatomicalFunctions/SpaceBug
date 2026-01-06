/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#include "StaticTranslationHandling.h"
#include "cmath"
#include "iostream"

namespace StaticTranslationHandling {
    /**
     * RIGHT -> UP
     * RIGHT -> DOWN
     * RIGHT -> LEFT
     * etc.
     */
    Utilities::Coordinates fromRightTranslation(Utilities::Coordinates coords) {
        if (coords.quadrant == Utilities::RIGHT) {
            return coords;
        } else if (coords.quadrant == Utilities::DOWN) {
            return ninetyTranslation(coords, 1);
        } else if (coords.quadrant == Utilities::LEFT) {
            return ninetyTranslation(coords, 2);
        } else if (coords.quadrant == Utilities::UP) {
            return ninetyTranslation(coords, 3);
        } else if (coords.quadrant == Utilities::DOWNRIGHT) {
            return fourFiveTranslation(coords);
        } else if (coords.quadrant == Utilities::DOWNLEFT) {
            return ninetyTranslation(fourFiveTranslation(coords), 1);
        } else if (coords.quadrant == Utilities::UPLEFT) {
            return ninetyTranslation(fourFiveTranslation(coords), 2);
        } else if (coords.quadrant == Utilities::UPRIGHT) {
            return ninetyTranslation(fourFiveTranslation(coords), 3);
        }
        cout << "Correct Quadrant Information Not Found" << endl;
        exit(EXIT_FAILURE); // -------------------------------------------------------------------------- EXIT PROGRAM
    }

    /**
     * UP -> RIGHT
     * DOWN -> RIGHT
     * LEFT -> RIGHT
     * etc.
     */
    Utilities::Coordinates toRightTranslation(Utilities::Coordinates coords,
                                              Utilities::Coordinates pos_coords) {
        coords.x -= pos_coords.x; //Positional translation done before rotation
        coords.y -= pos_coords.y;
        if (coords.quadrant == Utilities::RIGHT) {
            return coords;
        } else if (coords.quadrant == Utilities::UPRIGHT) {
            return ninetyTranslation(fourFiveBackTranslation(coords), 1);
        } else if (coords.quadrant == Utilities::UP) {
            return ninetyTranslation(coords, 1);
        } else if (coords.quadrant == Utilities::UPLEFT) {
            return ninetyTranslation(fourFiveBackTranslation(coords), 2);
        } else if (coords.quadrant == Utilities::LEFT) {
            return ninetyTranslation(coords, 2);
        } else if (coords.quadrant == Utilities::DOWNLEFT) {
            return ninetyTranslation(fourFiveBackTranslation(coords), 3);
        } else if (coords.quadrant == Utilities::DOWN) {
            return ninetyTranslation(coords, 3);
        } else if (coords.quadrant == Utilities::DOWNRIGHT) {
            return fourFiveBackTranslation(coords);
        }
        cout << "Correct Quadrant Information Not Found" << endl;
        exit(EXIT_FAILURE); // -------------------------------------------------------------------------- EXIT PROGRAM
    }

    /**
     * 90 DEGREE CLOCKWISE TRANSLATION
     */
    Utilities::Coordinates ninetyTranslation(Utilities::Coordinates coords, int runs) {
        for (int i = 0; i < runs; i++) {
            coords.set(coords.y, -coords.x);
        }
        return coords;
    }

    /**
     * RIGHT -> 45 DEGREES DOWN
     */
    Utilities::Coordinates fourFiveTranslation(Utilities::Coordinates coords) {
        coords.set(((sqrt(2) / 2) * (coords.x + coords.y)),
                   (-(sqrt(2) / 2) * (coords.x - coords.y)));
        return coords;
    }

    /**
     * 45 DEGREES DOWN -> RIGHT
     */
    Utilities::Coordinates fourFiveBackTranslation(Utilities::Coordinates coords) {
        coords.set(((coords.x - coords.y) / sqrt(2)),
                   ((coords.x + coords.y) / sqrt(2)));
        return coords;
    }

    /**
     * RIGHT -> 1 DEGREE DOWN
     */
    Utilities::Coordinates oneDegClockwiseTranslation(Utilities::Coordinates coords) {
        coords.set(((coords.x * cos(M_PI / 180)) + (coords.y * sin(M_PI / 180))),
                   (((-1 * coords.x) * sin(M_PI / 180)) + (coords.y * cos(M_PI / 180))));
        return coords;
    }

    /**
     * 1 DEGREE DOWN -> RIGHT
     */
    Utilities::Coordinates oneDegCounterTranslation(Utilities::Coordinates coords) {
        coords.set(((coords.x * cos(M_PI / 180)) - (coords.y * sin(M_PI / 180))),
                   ((coords.x * sin(M_PI / 180)) + (coords.y * cos(M_PI / 180))));
        return coords;
    }
}