/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#include "CollisionDetection.h"
#include "Utility Files/StaticTranslationHandling.h"
#include <iostream>
#include <cmath>

namespace CollisionDetection {

    bool checkCollision(
        Utilities::Coordinates c1,
        Utilities::Coordinates zero_coords_one,
        SpriteSize::Dimensions d1,
        int r1,
        Utilities::Coordinates c2,
        Utilities::Coordinates zero_coords_two,
        SpriteSize::Dimensions d2,
        int r2
        ) {

        //Logical operations:

        //      object one is flat, object two is flat:

        //two params total. Check range overlap in x and y

        //one_param.x_lesser <= two_param.x_lesser <= one_param.x_greater ||
        //one_param.x_lesser <= two_param.x_greater <= one_param.x_greater
        //Repeat for y, repeat for object two.

        //      object one is rotated, object two is flat:

        //the x coords of the four corners of object one is tested within the x range of object two if
        //y overlap is already confirmed.

        //if obj_one.top.y in range two_param.y_lesser - two_param.y_greater, then:
        //two_param.x_lesser <= obj_one.top.x <= two_param.x_greater
        //set two_param_one to false if failed.
        //repeat for three other points.

        //then, the other direction is:

        //      object one is flat, object two is rotated:

        //the two y coords of object one are used to generate x ranges from object two.
        //Two x coords are checked in each x range, four total.

        //if one_param.y_lesser in range obj_two.bottom.y - obj_two.top.y:
        //one_param.y_lesser -> two_param_one.x_lesser and two_param_one.x_greater are generated, fed one_param.x_lesser
        //                      two_param_two.x_lesser and two_param_two.x_greater are copied ^, fed one_param.x_greater
        //else: two_param_one and two_param_two default to "passed" (which is done on generation... no need for anything else)
        //if one_param.y_greater in range obj_two.bottom.y - obj_two.top.y:
        //one_param.y_greater -> two_param_three.x_lesser and two_param_three.x_greater are generated, fed one_param.x_lesser
        //                       two_param_four.x_lesser and two_param_four.x_greater are copied ^, fed one_param.x_greater
        //else: two_param_three and two_param_four default to "passed"

        //      both objects are rotated:

        //the y coords of the four corners of object one are used to generated four x ranges in object two,
        //this is after y overlap is first confirmed.
        //the x coordinates are tested in their respective ranges.
        //The same is done from object two to one, making for 8 x ranges, 16 generated x coords, and 8 comparisons total.

        //if obj_one.top.y in range obj_two.bottom.y - obj_two.top.y
        //obj_one.top.y -> two_param_one.x_lesser and two_param_one.x_greater are generated, fed obj_one.top.x
        //obj_one.bottom.y -> two_param_two.x_lesser and two_param_two.x_greater are generated, fed obj_one.bottom.x
        //obj_one.left.y -> two_param_three.x_lesser and two_param_three.x_greater are generated, fed obj_one.left.x
        //obj_one.right.y -> two_param_four.x_lesser and two_param_four.x_greater are generated, fed obj_one.right.x
        //Then the same in the opposite direction.

        RangeParameters
            obj_one_param_one,
            obj_one_param_two,
            obj_one_param_three,
            obj_one_param_four,
            obj_two_param_one,
            obj_two_param_two,
            obj_two_param_three,
            obj_two_param_four;
        ObjectPoints obj_one, obj_two;
        Utilities::SinAndCos sin_and_cos_one(r1), sin_and_cos_two(r2);

        bool objectOneFlat{false}, objectTwoFlat{false};

        if (flatRotation(r1)) {
            obj_one_param_one = evaluateRangesSimple(c1, zero_coords_one, d1, r1, sin_and_cos_one);
            objectOneFlat = true;
        } else {
            obj_one.evaluate(
                preCalculatedRotation(c1, zero_coords_one, sin_and_cos_one),
                preCalculatedRotation(Utilities::Coordinates{c1.x + d1.x, c1.y}, zero_coords_one, sin_and_cos_one),
                preCalculatedRotation(Utilities::Coordinates{c1.x, c1.y + d1.y}, zero_coords_one, sin_and_cos_one),
                preCalculatedRotation(Utilities::Coordinates{c1.x + d1.x, c1.y + d1.y}, zero_coords_one, sin_and_cos_one)
            );
        }

        if (flatRotation(r2)) {
            obj_two_param_one = evaluateRangesSimple(c2, zero_coords_two, d2, r2, sin_and_cos_two);
            objectTwoFlat = true;
        } else {
            obj_two.evaluate(
                preCalculatedRotation(c2, zero_coords_two, sin_and_cos_one),
                preCalculatedRotation(Utilities::Coordinates{c2.x + d2.x, c2.y}, zero_coords_two, sin_and_cos_two),
                preCalculatedRotation(Utilities::Coordinates{c2.x, c2.y + d2.y}, zero_coords_two, sin_and_cos_two),
                preCalculatedRotation(Utilities::Coordinates{c2.x + d2.x, c2.y + d2.y}, zero_coords_two, sin_and_cos_two)
            );
        }

        if (objectOneFlat && objectTwoFlat) {
            //falsify unused parameters.
            obj_one_param_two.passed = false;
            obj_one_param_three.passed = false;
            obj_one_param_four.passed = false;
            obj_two_param_two.passed = false;
            obj_two_param_three.passed = false;
            obj_two_param_four.passed = false;
            flatOnFlat(obj_one_param_one, obj_two_param_one);
            flatOnFlat(obj_two_param_one, obj_one_param_one);
        } else if (objectOneFlat && !objectTwoFlat) { //object two is rotated
            flatOnRotated(
                obj_one_param_one,
                obj_two,
                obj_two_param_one,
                obj_two_param_two,
                obj_two_param_three,
                obj_two_param_four
            );
            rotatedOnFlat(
                obj_two,
                obj_one_param_one,
                obj_one_param_two,
                obj_one_param_three,
                obj_one_param_four
            );
        } else if (!objectOneFlat && objectTwoFlat) { //object one is rotated
            rotatedOnFlat(
                obj_one,
                obj_two_param_one,
                obj_two_param_two,
                obj_two_param_three,
                obj_two_param_four
            );
            flatOnRotated(
                obj_two_param_one,
                obj_one,
                obj_one_param_one,
                obj_one_param_two,
                obj_one_param_three,
                obj_one_param_four
            );
        } else { //both are rotated
            rotatedOnRotated(
                obj_one,
                obj_two,
                obj_two_param_one,
                obj_two_param_two,
                obj_two_param_three,
                obj_two_param_four
            );
            rotatedOnRotated(
                obj_two,
                obj_one,
                obj_one_param_one,
                obj_one_param_two,
                obj_one_param_three,
                obj_one_param_four
            );
        }

        if (
            obj_one_param_one.passed ||
            obj_one_param_two.passed ||
            obj_one_param_three.passed ||
            obj_one_param_four.passed ||
            obj_two_param_one.passed ||
            obj_two_param_two.passed ||
            obj_two_param_three.passed ||
            obj_two_param_four.passed
        ) {
            return true;
        }
        return false;
    }

    ostream& operator<<(std::ostream& os, const PlainCoordinates& coords) {
        os << "(" << format("{:.4f}", coords.x) << ", " << format("{:.4f}", coords.y) << ")";
        return os;
    }

    bool PlainCoordinates::operator==(const PlainCoordinates& other) const {
        return (this->x == other.x && this->y == other.y);
    }

    bool ObjectPoints::checkContains(PlainCoordinates c) const {
        for (auto coords : points) {
            if (c.x == coords->x && c.y == coords->y) {
                return true;
            }
        }
        return false;
    }

    double ObjectPoints::calculateSlope(PlainCoordinates coords_one, PlainCoordinates coords_two) {
        return ((coords_two.y - coords_one.y) / (coords_two.x - coords_one.x));
    }

    double ObjectPoints::calculateIntercept(PlainCoordinates coords, double m) {
        return (coords.y - (m * coords.x));
    }

    void ObjectPoints::calculateSlopeIntercepts() {
        left_top.slope = calculateSlope(left, top);
        left_bottom.slope = calculateSlope(left, bottom);
        right_top.slope = calculateSlope(right, top);
        right_bottom.slope = calculateSlope(right, bottom);

        left_top.intercept = calculateIntercept(left, left_top.slope);
        left_bottom.intercept = calculateIntercept(left, left_bottom.slope);
        right_top.intercept = calculateIntercept(right, right_top.slope);
        right_bottom.intercept = calculateIntercept(right, right_bottom.slope);
    }

    void ObjectPoints::evaluate(
                PlainCoordinates c1,
                PlainCoordinates c2,
                PlainCoordinates c3,
                PlainCoordinates c4
            ) {
        array<PlainCoordinates, 4> points{c1, c2, c3, c4};
        for (auto coords : points) {
            if (top.y < coords.y || (top == PlainCoordinates{0, 0})) {
                top = coords;
            } if (bottom.y > coords.y || (bottom == PlainCoordinates{0, 0})) {
                bottom = coords;
            } if (left.x > coords.x || (left == PlainCoordinates{0, 0})) {
                left = coords;
            } if (right.x < coords.x || (right == PlainCoordinates{0, 0})) {
                right = coords;
            }
        }
        calculateSlopeIntercepts();
    }

    void rotatedOnRotated(
        ObjectPoints& obj_one,
        ObjectPoints& obj_two,
        RangeParameters& two_param_one,
        RangeParameters& two_param_two,
        RangeParameters& two_param_three,
        RangeParameters& two_param_four
        ) {
        RangeParameterXAssignment(obj_one.top.y, obj_two, two_param_one);
        RangeParameterXAssignment(obj_one.bottom.y, obj_two, two_param_two);
        RangeParameterXAssignment(obj_one.left.y, obj_two, two_param_three);
        RangeParameterXAssignment(obj_one.right.y, obj_two, two_param_four);
        flatOnRotatedEvaluation(two_param_one, obj_one.top.x);
        flatOnRotatedEvaluation(two_param_two, obj_one.bottom.x);
        flatOnRotatedEvaluation(two_param_three, obj_one.left.x);
        flatOnRotatedEvaluation(two_param_four, obj_one.right.x);
    }

    void flatOnRotated(
        RangeParameters& one_param,
        ObjectPoints& obj_two,
        RangeParameters& two_param_one,
        RangeParameters& two_param_two,
        RangeParameters& two_param_three,
        RangeParameters& two_param_four
        ) {
        RangeParameterXAssignment(one_param.y_lesser, obj_two, two_param_one);
        RangeParameterXAssignment(one_param.y_lesser, obj_two, two_param_two);
        RangeParameterXAssignment(one_param.y_greater, obj_two, two_param_three);
        RangeParameterXAssignment(one_param.y_greater, obj_two, two_param_four);

        flatOnRotatedEvaluation(two_param_one, one_param.x_lesser);
        flatOnRotatedEvaluation(two_param_two, one_param.x_greater);
        flatOnRotatedEvaluation(two_param_three, one_param.x_lesser);
        flatOnRotatedEvaluation(two_param_four, one_param.x_greater);
    }

    void flatOnRotatedEvaluation(
        RangeParameters& two_param,
        double x
        ) {
        if (two_param.passed && !checkOneRangeOverlap(two_param.x_lesser, two_param.x_greater, x)) {
            two_param.passed = false;
        }
    }

    void RangeParameterXAssignment(
        double y,
        ObjectPoints& obj_two,
        RangeParameters& two_param
        ) {
        if (checkOneRangeOverlap(
            obj_two.bottom.y,
            obj_two.top.y,
            y)
            ) {
            //within target y-range
            if (checkOneRangeOverlap(
                obj_two.left.y,
                obj_two.top.y,
                y)
                ) { //left-top
                two_param.x_lesser = calculateX(obj_two.left_top, y);
            } else { //left-bottom
                two_param.x_lesser = calculateX(obj_two.left_bottom, y);
            }
            if (checkOneRangeOverlap(
                obj_two.right.y,
                obj_two.top.y,
                y)
                ) { //right-top
                // x = (y - b) / m
                two_param.x_greater = calculateX(obj_two.right_top, y);
            } else { //right-bottom
                two_param.x_greater = calculateX(obj_two.right_bottom, y);
            }
        } else {
            two_param.passed = false;
        }
    }

    void rotatedOnFlat(
        ObjectPoints& obj_one,
        RangeParameters& two_param_one,
        RangeParameters& two_param_two,
        RangeParameters& two_param_three,
        RangeParameters& two_param_four
        ) {
        rotatedOnFlatIndividualComparison(obj_one.top, two_param_one, two_param_one);
        rotatedOnFlatIndividualComparison(obj_one.bottom, two_param_one, two_param_two);
        rotatedOnFlatIndividualComparison(obj_one.left, two_param_one, two_param_three);
        rotatedOnFlatIndividualComparison(obj_one.right, two_param_one, two_param_four);
    }

    void rotatedOnFlatIndividualComparison(
        PlainCoordinates& corner,
        RangeParameters& param,
        RangeParameters& affect
        ) {
        if (!checkOneRangeOverlap(
                param.y_lesser,
                param.y_greater,
                corner.y) ||
            !checkOneRangeOverlap(
                param.x_lesser,
                param.x_greater,
                corner.x)
            ) {
            affect.passed = false;
        }
    }

    void flatOnFlat(
        RangeParameters& param_one,
        RangeParameters& param_two
        ) {
        if (!checkTwoRangeOverlap(
                param_one.x_lesser,
                param_one.x_greater,
                param_two.x_lesser,
                param_two.x_greater) ||
            !checkTwoRangeOverlap(
                param_one.y_lesser,
                param_one.y_greater,
                param_two.y_lesser,
                param_two.y_greater)
            ) {
            param_two.passed = false;
        }
    }

    bool flatRotation(double r) {
        if (r == 0 ||
            r == 90 || r == -90 ||
            r == 360 || r == -360 ||
            r == 180 || r == -180 ||
            r == 270 || r == -270
            ) {
            return true;
            }
        return false;
    }

    double calculateX(slopeIntercept s_i, double y) {
        if (!s_i.slope == 0) {
            // x = (y - b) / m
            return ((y - s_i.intercept) / s_i.slope);
        }
        cout << "DIVISION BY ZERO - COLLISION DETECTION CALCULATEX" << endl;
        return 0;
    }

    RangeParameters evaluateRangesSimple(
        Utilities::Coordinates c1,
        Utilities::Coordinates zero_coords,
        SpriteSize::Dimensions d1,
        double r1,
        Utilities::SinAndCos sin_and_cos
        ) {
        RangeParameters obj;
        if (r1 == 90 || r1 == -270) {
            obj = {
                preCalculatedRotation(Utilities::Coordinates{c1.x, c1.y + d1.y}, zero_coords, sin_and_cos).x,
                preCalculatedRotation(c1, zero_coords, sin_and_cos).x,
                preCalculatedRotation(c1, zero_coords, sin_and_cos).y,
                preCalculatedRotation(Utilities::Coordinates{c1.x + d1.x, c1.y}, zero_coords, sin_and_cos).y
            };
        } else if (r1 == -90 || r1 == 270) {
            obj = {
                preCalculatedRotation(c1, zero_coords, sin_and_cos).x,
                preCalculatedRotation(Utilities::Coordinates{c1.x, c1.y + d1.y}, zero_coords, sin_and_cos).x,
                preCalculatedRotation(Utilities::Coordinates{c1.x + d1.x, c1.y}, zero_coords, sin_and_cos).y,
                preCalculatedRotation(c1, zero_coords, sin_and_cos).y
            };
        } else if (r1 == 180 || r1 == -180) {
            obj = {
                preCalculatedRotation(Utilities::Coordinates{c1.x + d1.x, c1.y}, zero_coords, sin_and_cos).x,
                preCalculatedRotation(c1, zero_coords, sin_and_cos).x,
                preCalculatedRotation(Utilities::Coordinates{c1.x, c1.y + d1.y}, zero_coords, sin_and_cos).y,
                preCalculatedRotation(c1, zero_coords, sin_and_cos).y
            };
        } else { //no rotation
            obj = {
                c1.x,
                c1.x + d1.x,
                c1.y,
                c1.y + d1.y
            };
        }
        return obj;
    }

    bool checkTwoRangeOverlap(
        double one_lesser,
        double one_greater,
        double two_lesser,
        double two_greater
        ) {
        if ((one_lesser <= two_lesser && two_lesser <= one_greater) ||
            (one_lesser <= two_greater && two_greater <= one_greater)) {
            return true;
        }
        return false;
    }

    bool checkOneRangeOverlap(
        double one_lesser,
        double one_greater,
        double two
        ) {
        if (one_lesser <= two && two <= one_greater) {
            return true;
        }
        return false;
    }

    PlainCoordinates preCalculatedRotation(
        Utilities::Coordinates coords,
        Utilities::Coordinates zero_coords,
        Utilities::SinAndCos s_c
    ) {
        coords.x -= zero_coords.x;
        coords.y -= zero_coords.y;
        coords.set(((coords.x * s_c.cos_v) - (coords.y * s_c.sin_v)),
                   ((coords.x * s_c.sin_v) + (coords.y * s_c.cos_v)));
        return PlainCoordinates{(coords.x + zero_coords.x), (coords.y + zero_coords.y)};
    }

    PlainCoordinates rotateObject(
        Utilities::Coordinates coords,
        Utilities::Coordinates zero_coords,
        double rotation
        ) {
        Utilities::SinAndCos s_c(rotation);
        coords.x -= zero_coords.x;
        coords.y -= zero_coords.y;
        coords.set(((coords.x * s_c.cos_v) - (coords.y * s_c.sin_v)),
                   ((coords.x * s_c.sin_v) + (coords.y * s_c.cos_v)));
        return PlainCoordinates{(coords.x + zero_coords.x), (coords.y + zero_coords.y)};
    }
}