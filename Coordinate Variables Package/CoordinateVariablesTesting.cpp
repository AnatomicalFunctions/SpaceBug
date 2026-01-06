/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#include "CoordinateVariables.h"
#include "CoordinateInterpretation.h"
#include "LengthAndSpacing.h"
#include <iostream>
#include "../Utility Files/Utilities.h"
using namespace std;

bool testCoordinateVariables();
bool testCoordinateInterpretation();
bool testLengthAndSpacing();
bool testTranslationHandling();

int main() {
    if (testCoordinateVariables()) {
        cout << "Passed all CV tests" << endl;
    }
    if (testCoordinateInterpretation()) {
        cout << "Passed all CI tests" << endl;
    }
    if (testLengthAndSpacing()) {
        cout << "Passed all LS tests" << endl;
    }
    if (testTranslationHandling()) {
        cout << "Passed all TH tests" << endl;
    }
}

bool testCoordinateVariables() {
    bool passed = true;

    Utilities::Coordinates startCoords{0, 0};
    Utilities::Coordinates endCoords{0, 0};
    double slope = 0;
    Utilities::Quadrants quadrant = Utilities::RIGHT;
    auto missile(CoordinateVariables(startCoords, endCoords, slope, quadrant));

    if (missile.getActualEndCoords().x != endCoords.x && missile.getActualEndCoords().x != endCoords.x) {
        passed = false;
    }

    return passed;
}

bool testCoordinateInterpretation() {
    bool passed = true;

    Utilities::Coordinates startCoords{0, 0};
    Utilities::Coordinates endCoords{0, 0};
    double slope = 0;
    Utilities::Quadrants quadrant = Utilities::RIGHT;
    auto missile(CoordinateVariables(startCoords, endCoords, slope, quadrant));

    missile.setA(0);
    missile.setSlope(0);
    if (missile.CI.endPointQuadraticCoordinateGenerator(10) != 0) {
        cout << "FAILED a = 0, slope = 0 f(x) test" << endl;
        passed = false;
    }
    missile.setA(0.075);
    missile.setSlope(0);
    if (missile.CI.endPointQuadraticCoordinateGenerator(10) != 7.5) {
        cout << "FAILED MAXRoC f(x) test" << endl;
        passed = false;
    }
    missile.setEndCoords(Utilities::Coordinates{-5, 0});
    missile.setA(0);
    missile.setSlope(0);
    missile.CI.evaluateA();
    if (missile.getA() == 0) {
        cout << "FAILED negative evaluateA() test" << endl;
        passed = false;
    }
    missile.setEndCoords(Utilities::Coordinates{0, 0});
    missile.setA(0);
    missile.setSlope(0);
    missile.CI.evaluateA();
    if (missile.getA() == 0) {
        cout << "FAILED 0, 0 evaluateA() test" << endl;
        passed = false;
    }
    missile.setEndCoords(Utilities::Coordinates{0, 0});
    missile.setA(-1);
    missile.setSlope(0);
    if (!missile.CI.exceedMaxRoC()) {
        cout << "FAILED exceedMaxRoC negative test" << endl;
        passed = false;
    }
    missile.setEndCoords(Utilities::Coordinates{0, 0});
    missile.setA(1);
    missile.setSlope(0);
    if (!missile.CI.exceedMaxRoC()) {
        cout << "FAILED exceedMaxRoC positive test" << endl;
        passed = false;
    }

    return passed;
}

bool testLengthAndSpacing() {
    bool passed = true;

    Utilities::Coordinates startCoords{0, 0};
    Utilities::Coordinates endCoords{0, 0};
    double slope = 0;
    Utilities::Quadrants quadrant = Utilities::RIGHT;
    auto missile(CoordinateVariables(startCoords, endCoords, slope, quadrant));

    missile.setStartCoords(Utilities::Coordinates{0, 0});
    missile.setEndCoords(Utilities::Coordinates{0, 0});
    missile.setA(0);
    missile.setSlope(0);
    if (missile.LS.arcLengthCalculator(10) != 10) {
        cout << "Failed arcLengthCalculator 0 test" << endl;
        passed = false;
    }
    missile.setA(0);
    missile.setSlope(0.25);
    missile.setEndCoords(Utilities::Coordinates{10, 2.5});
    //10^2 + 2.5^2 = ~10.3078
    if (!(missile.LS.arcLengthCalculator(10) > 10.3078 - CoordinateVariables::PRECISION_CONSTANT &&
          missile.LS.arcLengthCalculator(10) < 10.3078 + CoordinateVariables::PRECISION_CONSTANT )) {
        cout << "Failed arcLengthCalculator slope test" << endl;
        passed = false;
    }

    missile.setA(0.025);
    missile.setSlope(0.25);
    missile.setEndCoords(Utilities::Coordinates{10, 2.5});
    if (!(missile.LS.arcLengthCalculator(10) > 11.2549 - CoordinateVariables::PRECISION_CONSTANT &&
          missile.LS.arcLengthCalculator(10) < 11.2549 + CoordinateVariables::PRECISION_CONSTANT )) {
        cout << "Failed arcLengthCalculator arc test" << endl;
        passed = false;
    }

    missile.setA(0);
    missile.setSlope(0.25);
    missile.setEndCoords(Utilities::Coordinates{10, 2.5});
    if (!(missile.LS.slopeGen(10) > 0.25 - CoordinateVariables::PRECISION_CONSTANT &&
          missile.LS.slopeGen(10) < 0.25 + CoordinateVariables::PRECISION_CONSTANT)) {
        cout << "Failed slopeGen test" << endl;
        passed = false;
    }

    // missile.setA(CoordinateVariables::MAX_RATE_OF_CHANGE);
    // missile.setSlope(0);
    // missile.setEndCoords(Utilities::Coordinates{-10, 10});
    // //answer based on MAXRoC of 0.75
    // if (!(missile.LS.slopeBasedEndPoint() > 3.47 - CoordinateVariables::PRECISION_CONSTANT &&
    //       missile.LS.slopeBasedEndPoint() < 3.47 + CoordinateVariables::PRECISION_CONSTANT)) {
    //     cout << "Failed slopeBasedEndPoint MAXRoC test" << endl;
    //     passed = false;
    // }

    // missile.setA(CoordinateVariables::MAX_RATE_OF_CHANGE);
    // missile.setSlope(0);
    // missile.setEndCoords(Utilities::Coordinates{-10, 10});
    // if (!(missile.LS.slopeBasedRecursion(-1, CoordinateVariables::MAX_RATE_OF_CHANGE,
    //                                      CoordinateVariables::MAX_SLOPE) > 3.3355 - CoordinateVariables::PRECISION_CONSTANT &&
    //       missile.LS.slopeBasedRecursion(-1, CoordinateVariables::MAX_RATE_OF_CHANGE,
    //                                      CoordinateVariables::MAX_SLOPE) < 3.3355 + CoordinateVariables::PRECISION_CONSTANT)) {
    //     cout << "Failed slopeBasedEndPoint point of change test" << endl;
    //     passed = false;
    // }

    // missile.setA(CoordinateVariables::MAX_RATE_OF_CHANGE);
    // missile.setSlope(0);
    // missile.setEndCoords(Utilities::Coordinates{-10, 10});
    // if (!(missile.LS.numericRecursion(-1, 3, 3) > 2.91 - CoordinateVariables::PRECISION_CONSTANT &&
    //       missile.LS.numericRecursion(-1, 3, 3) < 2.91 + CoordinateVariables::PRECISION_CONSTANT)) {
    //     cout << "Failed numericRecursion specific length test" << endl;
    //     passed = false;
    // }

    missile.setA(0);
    missile.setSlope(0);
    missile.setEndCoords(Utilities::Coordinates{10, 0});
    if (!missile.LS.slopeBasedEndPoint() > 10 - CoordinateVariables::PRECISION_CONSTANT &&
        !missile.LS.slopeBasedEndPoint() < 10 + CoordinateVariables::PRECISION_CONSTANT) {
        cout << "Failed slopeBasedEndPoint no bound test" << endl;
        passed = false;
    }

    missile.setA(0.0075);
    missile.setSlope(0.6);
    missile.setEndCoords(Utilities::Coordinates{10, 0});
    if (missile.LS.slopeBasedEndPoint() != 0) {
        cout << missile.LS.slopeBasedEndPoint() << endl;
        cout << "Failed slopeBasedEndPoint exceed slope bound test" << endl;
        passed = false;
    }

    return passed;
}

bool testTranslationHandling() {
    bool passed = true;

    Utilities::Coordinates startCoords{0, 0};
    Utilities::Coordinates endCoords{0, 0};
    double slope = 0;
    Utilities::Quadrants quadrant = Utilities::RIGHT;
    auto missile(CoordinateVariables(startCoords, endCoords, slope, quadrant));

    //Should be updated for StaticTranslationHandling at some point:

    // Utilities::Coordinates test_coords{1, 0};
    // if (!(missile.TH.ninetyTranslation(test_coords, 1).x == 0 &&
    //       missile.TH.ninetyTranslation(test_coords, 1).y == -1)) {
    //     cout << "Failed ninetyTranslation DOWN test" << endl;
    //     passed = false;
    // }
    //
    // if (!(missile.TH.ninetyTranslation(test_coords, 2).x == -1 &&
    //       missile.TH.ninetyTranslation(test_coords, 2).y == 0)) {
    //     cout << "Failed ninetyTranslation LEFT test" << endl;
    //     passed = false;
    // }
    //
    // if (!(missile.TH.ninetyTranslation(test_coords, 3).x == 0 &&
    //       missile.TH.ninetyTranslation(test_coords, 3).y == 1)) {
    //     cout << "Failed ninetyTranslation UP test" << endl;
    //     passed = false;
    // }
    //
    // if (!((missile.TH.fourFiveTranslation(test_coords).x > 0.707 - CoordinateVariables::PRECISION_CONSTANT &&
    //        missile.TH.fourFiveTranslation(test_coords).x < 0.707 + CoordinateVariables::PRECISION_CONSTANT) &&
    //       (missile.TH.fourFiveTranslation(test_coords).y > -0.707 - CoordinateVariables::PRECISION_CONSTANT &&
    //        missile.TH.fourFiveTranslation(test_coords).y < -0.707 + CoordinateVariables::PRECISION_CONSTANT))) {
    //     cout << "Failed fourFive test" << endl;
    //     passed = false;
    // }
    //
    // if (!((missile.TH.fourFiveBackTranslation(test_coords).x > 0.707 - CoordinateVariables::PRECISION_CONSTANT &&
    //        missile.TH.fourFiveBackTranslation(test_coords).x < 0.707 + CoordinateVariables::PRECISION_CONSTANT) &&
    //       (missile.TH.fourFiveBackTranslation(test_coords).y > 0.707 - CoordinateVariables::PRECISION_CONSTANT &&
    //        missile.TH.fourFiveBackTranslation(test_coords).y < 0.707 + CoordinateVariables::PRECISION_CONSTANT))) {
    //     cout << "Failed fourFiveBack test" << endl;
    //     passed = false;
    // }

    return passed;
}