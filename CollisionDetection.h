/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include "Utility Files/SpriteSize.h"
#include "Utility Files/Utilities.h"

/**
 * Collection of functions used to determine if there is overlap between two rotated or flat rectangles in 2D space.
 * Uses RangeParameters struct as a bool...
 * if all RangeParameters are false by the end of the check, there is no overlap.
 */
namespace CollisionDetection {

    /**
     * Checks overlap between two objects in 2D space.
     * Calls rotation for the dimensions from base based on the quadrant of the objects.
     * @param c1 : The Coordinates of the first object
     * @param d1 : The Dimensions of the first object
     * @param r1 : The rotation of the first object
     * @param c2 : The Coordinates of the second object
     * @param d2 : The Dimensions of the second object
     * @param r2 : The rotation of the second object
     * @return : true if the border of one object is within the other.
     */
    bool checkCollision(
        Utilities::Coordinates c1,
        Utilities::Coordinates zero_coords_one,
        SpriteSize::Dimensions d1,
        int r1,
        Utilities::Coordinates c2,
        Utilities::Coordinates zero_coords_two,
        SpriteSize::Dimensions d2,
        int r2
    );

    /**
     * Coordinates structure without more data attached.
     */
    struct PlainCoordinates {
        double x{0};
        double y{0};
        bool operator==(const PlainCoordinates& other) const;
    };

    /**
     * Overloads the << operator to print the information for a given PlainCoordinates structure
     * @param os : the desired outstream
     * @param coords : a Coordinates structure
     * @return : the desired outstream, returned for continuation
     */
    std::ostream& operator<<(std::ostream& os, const PlainCoordinates& coords);

    /**
     * Defines range information as part of a given rectangle.
     * Only one is needed for a flat rectangle. Four are needed for a rotated rectangle.
     */
    struct RangeParameters {
        double x_lesser{0};
        double x_greater{0};
        double y_lesser{0};
        double y_greater{0};
        bool passed{true};
    };

    /**
     * Defines the information of a y = mx + b equation
     */
    struct slopeIntercept {
        double slope{0};
        double intercept{0};
    };

    /**
     * The corners of a rectangle.
     */
    struct ObjectPoints {
        PlainCoordinates top;
        PlainCoordinates bottom;
        PlainCoordinates left;
        PlainCoordinates right;
        array<PlainCoordinates*, 4> points{&top, &bottom, &left, &right};
        slopeIntercept left_top;
        slopeIntercept left_bottom;
        slopeIntercept right_top;
        slopeIntercept right_bottom;

        /**
         * @param c : a set of coordinates
         * @return : true if array "points" contains input "c"
         */
        bool checkContains(PlainCoordinates c) const;

        /**
         * @param coords_one : a PlainCoordinates struct
         * @param coords_two : a PlainCoordinates struct
         * @return : The slope connecting the two inputted points.
         */
        static double calculateSlope(PlainCoordinates coords_one, PlainCoordinates coords_two);

        /**
         * @return : b = y - mx
         */
        static double calculateIntercept(PlainCoordinates coords, double m);

        /**
         * Initializes the necessary slopeIntercept struct objects
         */
        void calculateSlopeIntercepts();

        /**
         * Evaluates four inputted coordinates based primarily on the y coordinate.
         * And calls calculateSlopeIntercepts();
         */
        void evaluate(
            PlainCoordinates c1,
            PlainCoordinates c2,
            PlainCoordinates c3,
            PlainCoordinates c4
        );
    };

    /**
     * Performs the necessary collision check operations for a rotated object checked against
     * another rotated object.
     */
    void rotatedOnRotated(
        ObjectPoints& obj_one,
        ObjectPoints& obj_two,
        RangeParameters& two_param_one,
        RangeParameters& two_param_two,
        RangeParameters& two_param_three,
        RangeParameters& two_param_four
    );

    /**
     * Performs the necessary collision check operations for a flat object checked against a rotated object.
     */
    void flatOnRotated(
        RangeParameters& one_param,
        ObjectPoints& obj_two,
        RangeParameters& two_param_one,
        RangeParameters& two_param_two,
        RangeParameters& two_param_three,
        RangeParameters& two_param_four
    );

    /**
     * Final evaluation of a RangeParameter
     */
    void flatOnRotatedEvaluation(
        RangeParameters& two_param,
        double x
    );

    /**
     * Assigns x values to various RangeParameters given particular y values.
     */
    void RangeParameterXAssignment(
        double y,
        ObjectPoints& obj_two,
        RangeParameters& two_param
    );

    /**
     * Performs the necessary collision check operation for a rotated object checked against a flat object.
     */
    void rotatedOnFlat(
        ObjectPoints& obj_one,
        RangeParameters& two_param_one,
        RangeParameters& two_param_two,
        RangeParameters& two_param_three,
        RangeParameters& two_param_four
    );

    /**
     * Individual range checks for rotated objects on flat objects
     */
    void rotatedOnFlatIndividualComparison(
        PlainCoordinates& corner,
        RangeParameters& param,
        RangeParameters& affect
    );

    /**
     * Performs the necessary collision check operation for a flat object checked against another flat object.
     */
    void flatOnFlat(RangeParameters& param_one, RangeParameters& param_two);

    /**
     * @return: true if a given rotation results in a flat shape.
     */
    bool flatRotation(double r);

    /**
     * Finds x = (y - b) / m
     */
    double calculateX(slopeIntercept s_i, double y);

    /**
     * Evaluates the ranges for a given flat rectangle.
     */
    RangeParameters evaluateRangesSimple(
        Utilities::Coordinates c1,
        Utilities::Coordinates zero_coords,
        SpriteSize::Dimensions d1,
        double r1,
        Utilities::SinAndCos sin_and_cos
    );

    /**
     * Bool check for the overlap of two ranges
     */
    bool checkTwoRangeOverlap(
        double one_lesser,
        double one_greater,
        double two_lesser,
        double two_greater
    );

    /**
     * Bool check for the overlap of a single coordinate and a range.
     */
    bool checkOneRangeOverlap(
        double one_lesser,
        double one_greater,
        double two
    );

    /**
     * @param coords : point being rotated
     * @param zero_coords : point of rotation
     * @param s_c : desired rotation in SinAndCos
     */
    PlainCoordinates preCalculatedRotation(
        Utilities::Coordinates coords,
        Utilities::Coordinates zero_coords,
        Utilities::SinAndCos s_c
    );

    /**
     * @param coords : point being rotated
     * @param zero_coords : point of rotation
     * @param rotation : desired rotation in degrees
     */
    PlainCoordinates rotateObject(
        Utilities::Coordinates coords,
        Utilities::Coordinates zero_coords,
        double rotation
    );
}

#endif //COLLISIONDETECTION_H