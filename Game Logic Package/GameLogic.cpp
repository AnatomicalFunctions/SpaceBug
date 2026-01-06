/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#include "GameLogic.h"
#include "../CollisionDetection.h"
#include <iostream>

using namespace std;

GameLogic::GameLogic(
    sf::RenderWindow& window,
    vector<sf::Vertex>& vertexArray,
    double resolutionScale
    ) :
    vertexArray(vertexArray),
    resolutionScale(resolutionScale),
    EH(window, this),
    RO(vertexArray, resolutionScale, *this) {
}

void GameLogic::checkEvents() const {
    EH.checkEvents();
}

void GameLogic::update() {
    if (getCurrentRound() == ROUND_ONE) {
        RO.update();
    }
}

void GameLogic::input(SpaceCraft::InputType inputType) {
    if (getCurrentRound() == ROUND_ONE) {
        RO.getSpaceCraft().input(inputType);
    }
}

void GameLogic::chaff() {
    RO.chaff();
}

void GameLogic::gunfire() {
    RO.gunfire();
}

void GameLogic::resetGunfireIncrement() {
    RO.resetGunfireIncrement();
}

void GameLogic::explode() {
    RO.explode();
}

//Hand it the coords you want the center to be... Don't force anything prior to input
//Center is defined as middle-left
void GameLogic::appendArray(
    const Utilities::Coordinates &interpolation,
    const SpriteSize::Dimensions &dimensions,
    double rotation,
    const sf::Color &color
    ) const {

    Utilities::SinAndCos s_c(rotation);
    CollisionDetection::PlainCoordinates c_i_1{
        CollisionDetection::preCalculatedRotation(
            {interpolation.x, interpolation.y - (dimensions.y / 2.0)},
            interpolation,
            s_c
        )};
    CollisionDetection::PlainCoordinates c_i_2{
        CollisionDetection::preCalculatedRotation(
            {interpolation.x, (interpolation.y + dimensions.y / 2.0)},
            interpolation,
            s_c
        )};
    CollisionDetection::PlainCoordinates c_i_3{
        CollisionDetection::preCalculatedRotation(
            {interpolation.x + dimensions.x, interpolation.y - (dimensions.y / 2.0)},
            interpolation,
            s_c
        )};
    CollisionDetection::PlainCoordinates c_i_4{
        CollisionDetection::preCalculatedRotation(
            {interpolation.x + dimensions.x, (interpolation.y + dimensions.y / 2.0)},
            interpolation,
            s_c
        )};

    vertexArray.push_back(sf::Vertex({float(c_i_1.x * resolutionScale), float(c_i_1.y * resolutionScale)}, color));
    vertexArray.push_back(sf::Vertex({float(c_i_2.x * resolutionScale), float(c_i_2.y * resolutionScale)}, color));
    vertexArray.push_back(sf::Vertex({float(c_i_3.x * resolutionScale), float(c_i_3.y * resolutionScale)}, color));

    vertexArray.push_back(sf::Vertex({float(c_i_4.x * resolutionScale), float(c_i_4.y * resolutionScale)}, color));
    vertexArray.push_back(sf::Vertex({float(c_i_2.x * resolutionScale), float(c_i_2.y * resolutionScale)}, color));
    vertexArray.push_back(sf::Vertex({float(c_i_3.x * resolutionScale), float(c_i_3.y * resolutionScale)}, color));
}

void GameLogic::reset() { //very temporary... gonna remove later
    RO.reset();
}

double GameLogic::getResolutionScale() const {
    return resolutionScale;
}

GameLogic::Round GameLogic::getCurrentRound() const {
    return currentRound;
}

void GameLogic::setCurrentRound(Round currentRound) {
    this->currentRound = currentRound;
}
