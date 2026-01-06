/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#include <format>
#include <iomanip>
#include <iostream>
#include <memory>
#include "Utilities.h"
#include "../Coordinate Variables Package/CoordinateInterpretation.h"
#include "../Coordinate Variables Package/CoordinateVariables.h"
#include "../Coordinate Variables Package/LengthAndSpacing.h"
#include "../CollisionDetection.h"
#include "../Missile.h"
#include "../SpaceCraft.h"
using namespace std;

Utilities::Coordinates::Coordinates(
    double x,
    double y,
    double a,
    double slope,
    Quadrants quadrant,
    double length
    ) :
    x(x),
    y(y),
    a(a),
    slope(slope),
    quadrant(quadrant),
    length(length) {
}

Utilities::Coordinates::Coordinates(
    const CollisionDetection::PlainCoordinates& other
    ) :
    x(other.x),
    y(other.y) {
}

sf::VideoMode Utilities::checkAspectRatio() {
    //Checks if aspect ratio of desktop is 16:9
    std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
    for (auto mode : modes) {
        if ((mode.size.x / double(mode.size.y)) > (640.0 / 360.0) - 0.001 &&
            (mode.size.x / double(mode.size.y)) < (640.0 / 360.0) + 0.001) {
            return mode;
        }
    }
    cout << "16:9 Aspect Ratio Not Found" << endl;
    exit(EXIT_FAILURE); // -------------------------------------------------------------------------- EXIT PROGRAM
}

bool Utilities::goForChaff(Missile missile, const SpaceCraft& s_craft) {
    if (!s_craft.getChaffExist()) {
        return false;
    }
    missile.newTargetPosition(s_craft.getChaffCenter());
    if (!missile.targetInQuadrant(missile.LS.slopeBasedEndPoint())) {
        return false;
    }

    double length_to_target = missile.getCurrentInterpolation().length;
    double length_to_chaff = missile.interpolation().length;

    if (length_to_chaff < length_to_target) {
        return true;
    }
    return false;
}

bool Utilities::goForTarget(Missile missile, Coordinates target) {
    missile.newTargetPosition(target);
    if (!missile.targetInQuadrant(missile.LS.slopeBasedEndPoint())) {
        return false;
    }
    return true;
}

double Utilities::convertToDegrees(Coordinates coords) {
    double degrees = (coords.slope * 45);
    for (int i = 0; i < int(coords.quadrant); i++) {degrees -= 45;}
    if (coords.quadrant == RIGHT && degrees > 0) {degrees -= 360;} //ensures negative return value
    return degrees;
}

Utilities::QuadrantAndSlope Utilities::convertToQuadrantAndSlope(int rotation) {
    while (rotation > 0) {rotation -= 360;}
    while (rotation <= -360) {rotation += 360;}
    //works with negatives only
    Quadrants quadrant = Quadrants((rotation - 22.5) / -45);
    double slope = ((rotation + (int(quadrant) * 45)) / 45.0);
    //corrects quadrant after slope has been calculated with 8.
    if (quadrant == 8) {quadrant = RIGHT;}
    return {quadrant, slope};
}

ostream& operator<<(std::ostream& os, const Utilities::Coordinates& coords) {
    os << "(" << format("{:.4f}", coords.x) << ", " << format("{:.4f}", coords.y) << ")";
    return os;
}

double Utilities::getDoubleFromUser(ostream &outs, istream &ins, string prompt) {
    // I used two istringstream calls (s1, s2) so I could extract twice to test over multiple variables.
    // string "line" is copied into string "line2" so the extractions do not overlap.
    string line, line2, extra;
    double number;
    outs << prompt;
    getline(ins, line);
    line2 = line;
    istringstream s1(line);
    istringstream s2(line2);
    // while ((The input is not a double) or (The input is a double, but there is more following))
    while (!(s1 >> number) || s2 >> number >> extra) {
        if (line.size() == 0) {
            outs << "No input. Enter a number: ";
        }
        else {
            outs << "Invalid input. Enter a number: ";
        }
        s1.clear();
        s2.clear();
        getline(ins, line);
        line2 = line;
        s1.str(line);
        s2.str(line2);
    }
    return number;
}

int Utilities::getIntFromUser(ostream &outs, istream &ins, string prompt) {
    // I used two istringstream calls (s1, s2) so I could extract twice to test over multiple variables.
    // string "line" is copied into string "line2" so the extractions do not overlap.
    string line, line2, extra;
    int number;
    outs << prompt;
    getline(ins, line);
    line2 = line;
    istringstream s1(line);
    istringstream s2(line2);
    // while ((The input is not an integer) or (The input is an integer, but there is more following))
    while (!(s1 >> number) || s2 >> number >> extra) {
        if (line.size() == 0) {
            outs << "No input. Enter a number: ";
        }
        else {
            outs << "Invalid input. Enter a number: ";
        }
        s1.clear();
        s2.clear();
        getline(ins, line);
        line2 = line;
        s1.str(line);
        s2.str(line2);
    }
    return number;
}