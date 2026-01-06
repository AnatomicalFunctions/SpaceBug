/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#ifndef UTILITIES_H
#define UTILITIES_H

#include <math.h>
#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>
#include <iostream>
using namespace std;
namespace CollisionDetection {
    struct PlainCoordinates;   // forward declaration
}

class SpaceCraft;
class Missile;
class CoordinateVariables;

/**
 * Class of functions of nonspecific or main() use
 */
class Utilities {
public:
    //specific clockwise order, see setQuadrant()
    enum Quadrants{RIGHT, DOWNRIGHT, DOWN, DOWNLEFT, LEFT, UPLEFT, UP, UPRIGHT};
    enum Direction {LEFT_TURN = 1, RIGHT_TURN = -1};

    /**
     * A representation of rotation, expressed in slope and quadrant form.
     */
    struct QuadrantAndSlope {
        Quadrants quadrant;
        double slope;
    };

    /**
     * Structure for containing a set of coordinates, x and y.
     * Also contains information about the position of the coordinate relative to its arc and quadrant:
     * "a", slope, and quadrant
     * Function "set" allows for reassignment of x and y variables for use in translation.
     */
    struct Coordinates {
        double x{0};
        double y{0};
        double a{0};
        double slope{0};
        Quadrants quadrant{RIGHT};
        double length{0};

        Coordinates(
            double x = 0,
            double y = 0,
            double a = 0,
            double slope = 0,
            Quadrants quadrant = RIGHT,
            double length = 0
        );

        Coordinates(const CollisionDetection::PlainCoordinates& coords);

        void set(
            double x,
            double y
            ) {
            this->x = x;
            this->y = y;
        }

        bool operator==(const Coordinates& other) const {
            return (this->x == other.x && this->y == other.y);
        }
    };

    struct SinAndCos {
        double sin_v;
        double cos_v;

        SinAndCos(double rotation) {
            this->sin_v = sin(sf::degrees(rotation).asRadians());
            this->cos_v = cos(sf::degrees(rotation).asRadians());
        }
    };

    /**
     * Bundles together information. Used when passed into CV through several classes.
     */
    struct CVBundle {
        Coordinates startCoords;
        Coordinates endCoords;
        double MAX_RATE_OF_CHANGE;
        double MAX_SEGMENT_LENGTH;
    };

    /**
     * Checks the available aspect ratios of the user's computer using the SFML library.
     * If there is no available 16:9 mode, the program exits.
     * @return : the highest available quality display.
     */
    static sf::VideoMode checkAspectRatio();

    static bool goForChaff(Missile missile, const SpaceCraft& s_craft);

    static bool goForTarget(Missile missile, Coordinates target);

    /**
     * Takes a Coordinates struct and uses the embedded slope and quadrant information to determine the
     * current angle in degrees.
     * @param coords : a Coordinates struct
     * @return : Current angle in negative degrees.
     */
    static double convertToDegrees(Coordinates coords);

    /**
     * @param rotation : angle in degrees
     * @return : angle in quadrant and slope
     */
    static QuadrantAndSlope convertToQuadrantAndSlope(int rotation);

    /**
     * Overloads the << operator to print the information for a given Coordinates structure
     * @param os : the desired outstream
     * @param coords : a Coordinates structure
     * @return : the desired outstream, returned for continuation
     */
    friend std::ostream& operator<<(std::ostream& os, const Coordinates& coords);

    /**
     * Function for validating the input of doubles
     * @param outs : the specified outstream
     * @param ins : the specified instream
     * @param prompt : a string representing specific feedback to the user
     * @return : a double
     */
    static double getDoubleFromUser(ostream &outs, istream &ins, string prompt);

    /**
     * Function for validating the input of integers
     * @param outs : the specified outstream
     * @param ins : the specified instream
     * @param prompt : a string representing specific feedback to the user
     * @return : an int
     */
    static int getIntFromUser(ostream &outs, istream &ins, string prompt);
};

#endif //UTILITIES_H
