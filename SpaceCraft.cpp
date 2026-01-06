/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#include "SpaceCraft.h"
#include "CollisionDetection.h"
#include "Utility Files/SpriteSize.h"
#include "Game Logic Package/GameLogic.h"
#include <cmath>
#include <iostream>

SpaceCraft::SpaceCraft(
    vector<sf::Vertex>& vertexArray,
    const double resolutionScale,
    GameLogic& game,
    Utilities::Coordinates startCoords,
    double ROC_ONE,
    double ROC_TWO,
    double MAX_SEGMENT_LENGTH
    ) :
    vertexArray(vertexArray),
    resolutionScale(resolutionScale),
    game(game),
    ROC_ONE(ROC_ONE),
    ROC_TWO(ROC_TWO),
    CoordinateVariables( //Calls a special SpaceCraft constructor in CV
        startCoords,
        ROC_TWO,
        MAX_SEGMENT_LENGTH
    ) {
    SpaceCraft::initialization();
}

void SpaceCraft::update() {
    if (!finished_head) {
        if (currentInterpolation.x > SpriteSize::Resolution.x || currentInterpolation.x < 0) {
            impactBorderLeftRight();
        } else if (currentInterpolation.y > SpriteSize::Resolution.y || currentInterpolation.y < 0) {
            impactBorderUpDown();
        }

        if (!pastInterpolations.empty() && (pastInterpolations.size() > MAX_TRAIL_LENGTH ||
            (increment % 2 == 0 && pastInterpolations.size() < MAX_TRAIL_LENGTH))
            ) {
            pastInterpolations.erase(pastInterpolations.begin());
            }
        pastInterpolations.push_back(currentInterpolation);
        setCurrentInterpolation(interpolation());
        rotation = Utilities::convertToDegrees(currentInterpolation);
        render();
    }

    if (!finished_tail) {
        if (finished_head) {
            if (pastInterpolations.empty()) {
                finished_tail = true;
            } else if (!pastInterpolations.empty() && increment % 2 == 0) {
                pastInterpolations.erase(pastInterpolations.begin());
            }
        }

        sf::Color colorTrail(0, 0, 60);
        sf::Color colorEndTrail(0, 0, 0);
        sf::Color color;
        SpriteSize::Dimensions trailSprite{3, 1};
        for (int i = 0; i < pastInterpolations.size(); i++) {

            if (i < pastInterpolations.size() - 20) {
                colorEndTrail.b += 12;
                color = colorEndTrail;
            } else {
                colorTrail.r += 12;
                colorTrail.g += 6;
                colorTrail.b += 3;
                color = colorTrail;
            }

            game.appendArray(
                pastInterpolations[i],
                trailSprite,
                (Utilities::convertToDegrees(pastInterpolations[i])),
                color
            );
        }
    }

    finished_animation = true;
    for (int i = 0; i < explosion.size(); i++) {
        if (!explosion[i]->getFinished()) {
            explosion[i]->update();
            finished_animation = false;
        } else {
            explosion.erase(explosion.begin() + i);
            i--;
        }
    }

    for (int i = 0; i < chaff.size(); i++) {
        if (!chaff[i]->getFinished()) {
            chaff[i]->update();
        } else {
            chaffErase(i);
            i--;
        }
    }

    if (chaff.empty()) {
        chaffExist = false;
    } else {
        chaffExist = true;
    }

    for (int i = 0; i < gunfire.size(); i++) {
        if (!gunfire[i]->getFinished()) {
            gunfire[i]->update();
        } else {
            gunfire.erase(gunfire.begin() + i);
            i--;
        }
    }

    if (gunfire.empty()) {
        gunfireExist = false;
    } else {
        gunfireExist = true;
    }

    if (!gunfireExist &&
        !chaffExist &&
        finished_animation &&
        finished_head &&
        finished_tail
        ) {
        finished = true;
    }

    increment++;
}

void SpaceCraft::render() {
    Utilities::SinAndCos sin_and_cos(rotation);

    appendTriangleArray(
        currentInterpolation,
        {currentInterpolation.x, currentInterpolation.y},
        {currentInterpolation.x, currentInterpolation.y + (SpriteSize::SpaceCraft.y / 2.0)},
        {currentInterpolation.x  + (SpriteSize::SpaceCraft.x / 2.0), currentInterpolation.y},
        sin_and_cos,
        sf::Color(186, 186, 186)
    );

    appendTriangleArray(
        currentInterpolation,
        {currentInterpolation.x, currentInterpolation.y},
        {currentInterpolation.x, currentInterpolation.y - (SpriteSize::SpaceCraft.y / 2.0)},
        {currentInterpolation.x + (SpriteSize::SpaceCraft.x / 2.0), currentInterpolation.y},
        sin_and_cos,
        sf::Color(186, 186, 186)
    );

    appendTriangleArray(
        currentInterpolation,
        {currentInterpolation.x, currentInterpolation.y},
        {currentInterpolation.x, currentInterpolation.y + (SpriteSize::SpaceCraft.y / 4.0)},
        {currentInterpolation.x + SpriteSize::SpaceCraft.x, currentInterpolation.y},
        sin_and_cos,
        sf::Color(212, 255, 233)
    );

    appendTriangleArray(
        currentInterpolation,
        {currentInterpolation.x, currentInterpolation.y},
        {currentInterpolation.x, currentInterpolation.y - (SpriteSize::SpaceCraft.y / 4.0)},
        {currentInterpolation.x + SpriteSize::SpaceCraft.x, currentInterpolation.y},
        sin_and_cos,
        sf::Color(212, 255, 233)
    );
}

void SpaceCraft::impact() {
    if (!finished_head) {
        finished_head = true;
        explosion.push_back(make_unique<CraftExplosion>(
            vertexArray, resolutionScale, game,
            getCurrentInterpolation(),
            Utilities::convertToDegrees(getCurrentInterpolation())));
        setCurrentInterpolation({-100, -100});
    }
}

void SpaceCraft::impactBorderUpDown() {
    if (!finished_head) {
        finished_head = true;
        explosion.push_back(make_unique<CraftExplosion>(
            vertexArray, resolutionScale, game,
            getCurrentInterpolation(),
            -Utilities::convertToDegrees(getCurrentInterpolation())));
        setCurrentInterpolation(Utilities::Coordinates{-100, -100});
    }
}

void SpaceCraft::impactBorderLeftRight() {
    if (!finished_head) {
        finished_head = true;
        explosion.push_back(make_unique<CraftExplosion>(
            vertexArray, resolutionScale, game,
            getCurrentInterpolation(),
            -Utilities::convertToDegrees(getCurrentInterpolation()) + 180));
        setCurrentInterpolation(Utilities::Coordinates{-100, 100});
    }
}

void SpaceCraft::reset(
    Utilities::Coordinates startCoords
    ) {
    setCurrentInterpolation({});
    increment = 0;
    gunfire_increment = 0;
    gunfire_spread = 0;
    gunfire_direction_switch = false;
    rotation = 0;
    right_depressed = false;
    left_depressed = false;
    chaffExist = false;
    gunfireExist = false;
    finished_head = false;
    finished_tail = false;
    finished_animation = false;
    finished = false;
    gunfire.clear();
    pastInterpolations.clear();
    explosion.clear();
    chaff.clear();
    CoordinateVariables::reset(startCoords, {});
}

void SpaceCraft::appendTriangleArray(
    const Utilities::Coordinates &zero_point,
    const Utilities::Coordinates &point_one,
    const Utilities::Coordinates &point_two,
    const Utilities::Coordinates &point_three,
    const Utilities::SinAndCos &sin_and_cos,
    sf::Color color
    ) const {
    CollisionDetection::PlainCoordinates c_i_1 =
        {CollisionDetection::preCalculatedRotation(
            point_one, zero_point, sin_and_cos)};
    CollisionDetection::PlainCoordinates c_i_2 =
        {CollisionDetection::preCalculatedRotation(
            point_two, zero_point, sin_and_cos)};
    CollisionDetection::PlainCoordinates c_i_3 =
        {CollisionDetection::preCalculatedRotation(
            point_three, zero_point, sin_and_cos)};

    vertexArray.push_back(sf::Vertex({float(c_i_1.x * resolutionScale), float(c_i_1.y * resolutionScale)}, color));
    vertexArray.push_back(sf::Vertex({float(c_i_2.x * resolutionScale), float(c_i_2.y * resolutionScale)}, color));
    vertexArray.push_back(sf::Vertex({float(c_i_3.x * resolutionScale), float(c_i_3.y * resolutionScale)}, color));
}

void SpaceCraft::input(InputType inputType) {
    //Erases unused arc-builder data
    setVCoordsClearAfterIndex(getInterpolationIncrement()); //matches current interpolationEndCoords
    setStartCoords(getVCoords().back());
    setQuadrant(getStartCoords().quadrant);
    setSlope(getStartCoords().slope);
    if (inputType == DEPRESS_LEFT &&
        !left_depressed &&
        !right_depressed
        ) {
        depression(getInterpolationIncrement(), Utilities::LEFT_TURN);
        left_depressed = true;
    } else if (inputType == DEPRESS_RIGHT &&
        !left_depressed &&
        !right_depressed
        ) {
        depression(getInterpolationIncrement(), Utilities::RIGHT_TURN);
        right_depressed = true;
    } else if (inputType == RELEASE_LEFT &&
        left_depressed &&
        !right_depressed
        ) {
        release(getInterpolationIncrement(), Utilities::LEFT_TURN);
        left_depressed = false;
    } else if (inputType == RELEASE_RIGHT &&
        right_depressed &&
        !left_depressed
        ) {
        release(getInterpolationIncrement(), Utilities::RIGHT_TURN);
        right_depressed = false;
    }
}

//Pulls information from the Coordinates structure to determine location
//And thus the right course of action.
void SpaceCraft::depression(int index, int direction_modifier) {
    if (getVCoords()[index].a == (direction_modifier * ROC_ONE)) {
        setA(ROC_TWO);
        rocArcGenRecursion(MAX_ARC_LENGTH);
    } else if (getVCoords()[index].a == (direction_modifier * -ROC_ONE)) {
        setA(0);
        rocArcGenRecursion(MAX_ARC_LENGTH);
        turnOut(direction_modifier);
    } else if (getVCoords()[index].a == (direction_modifier * -ROC_TWO)) {
        turnIn(-direction_modifier);
        turnOut(direction_modifier);
    } else {
        turnOut(direction_modifier);
    }
}

void SpaceCraft::release(int index, int direction_modifier) {
    if (getVCoords()[index].a == (direction_modifier * ROC_ONE)) {
        setA(0); //still on 1/-1, so straight back to 0
        rocArcGenRecursion(MAX_ARC_LENGTH);
    } else {
        turnIn(direction_modifier);
    }
}

//Inward toward straight from a RIGHT or LEFT turn
void SpaceCraft::turnIn(int direction_modifier) {
    setA(ROC_ONE * direction_modifier);
    rocArcGenRecursion(MAX_ARC_LENGTH);
    setA(0);
    rocArcGenRecursion(MAX_ARC_LENGTH);
}

//Outward from straight toward a RIGHT or LEFT turn
void SpaceCraft::turnOut(int direction_modifier) {
    setA(ROC_ONE * direction_modifier);
    rocArcGenRecursion(MAX_ARC_LENGTH);
    setA(ROC_TWO * direction_modifier);
    rocArcGenRecursion(MAX_ARC_LENGTH);
}

void SpaceCraft::initialization() {
    setA(0);
    rocArcGenRecursion(MAX_ARC_LENGTH);
}

void SpaceCraft::arcGenCall() {
    rocArcGenRecursion(MAX_ARC_LENGTH);
}

void SpaceCraft::chaffGen() {
    if (chaff.empty()) {
        chaff.push_back(make_unique<Chaff>(vertexArray, resolutionScale, game, *this));
    }
}

void SpaceCraft::gunfireGen() {
    if (gunfire_increment % 6 == 0 || gunfire_increment == 0) {
        Utilities::Coordinates fire_gen;
        if (gunfire_direction_switch) {
            gunfire_direction_switch = false;
            gunfire_spread *= -1;
            fire_gen =
                CollisionDetection::rotateObject(
                    {currentInterpolation.x + 6, currentInterpolation.y + 1},
                    currentInterpolation,
                    rotation
                );
        } else {
            gunfire_direction_switch = true;
            gunfire_spread *= -1;
            fire_gen =
                CollisionDetection::rotateObject(
                    {currentInterpolation.x + 6, currentInterpolation.y - 1},
                    currentInterpolation,
                    rotation
                );
        }
        fire_gen.slope = currentInterpolation.slope;
        fire_gen.quadrant = currentInterpolation.quadrant;
        gunfire.push_back(make_unique<Gunfire>(vertexArray, resolutionScale, game, fire_gen, gunfire_spread));
    }
    gunfire_increment++;
    if (gunfire_spread < 0) {gunfire_spread -= 5;} else {gunfire_spread += 5;}
    if (gunfire_spread > 30 || gunfire_spread < -30) {gunfire_spread = 0;}
}

void SpaceCraft::resetGunfireIncrement() {
    gunfire_increment = 0;
}

void SpaceCraft::chaffErase(int i) {
    chaff.erase(chaff.begin() + i);
}

Utilities::Coordinates SpaceCraft::getCurrentInterpolation() const {
    return currentInterpolation;
}

double SpaceCraft::getRotation() const {
    return rotation;
}

bool SpaceCraft::getFinished() const {
    return finished;
}

Utilities::Coordinates SpaceCraft::getChaffCenter() const {
    return chaff[0]->getCenter();
}

bool SpaceCraft::getChaffExist() const {
    return chaffExist;
}

const vector<unique_ptr<Gunfire>>& SpaceCraft::getGunfire() const {
    return gunfire;
}

void SpaceCraft::setCurrentInterpolation(Utilities::Coordinates currentInterpolation) {
    this->currentInterpolation = currentInterpolation;
}