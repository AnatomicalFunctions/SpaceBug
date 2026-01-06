/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#include "../Utility Files/StaticTranslationHandling.h"
#include "CoordinateVariables.h"
#include "CoordinateInterpretation.h"
#include "LengthAndSpacing.h"
#include "../Coordinate Variables Package/TranslationHandling.h"
#include <cmath>
#include <iostream>
using namespace std;

TranslationHandling::TranslationHandling(CoordinateVariables* CV) : CV(CV) {}

void TranslationHandling::setptrs() {
    this->CI = &CV->CI;
    this->LS = &CV->LS;
}

Utilities::Coordinates TranslationHandling::translateArc(double x) const {
    Utilities::Coordinates coords(
        x,
        CI->endPointQuadraticCoordinateGenerator(x),
        CV->getA(),
        LS->slopeGen(x),
        CV->getQuadrant(),
        CV->getLength()
    );
    coords = StaticTranslationHandling::fromRightTranslation(coords);
    coords = positionalTranslation(coords); //positional isn't based on quadrants, so rotation must occur first.
    return coords;
}

Utilities::Coordinates TranslationHandling::positionalTranslation(Utilities::Coordinates coords) const {
    coords.x += CV->getStartCoords().x;
    coords.y += CV->getStartCoords().y;
    return coords;
}