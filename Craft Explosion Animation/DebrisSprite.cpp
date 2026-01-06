/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#include "DebrisSprite.h"
#include "../CollisionDetection.h"
#include "../Game Logic Package/GameLogic.h"
#include <iostream>
#include "../Utility Files/SpriteSize.h"
using namespace std;

DebrisSprite::DebrisSprite(
    vector<sf::Vertex>& vertexArray,
    const double resolutionScale,
    GameLogic& game,
    Utilities::Coordinates startCoords,
    Utilities::Coordinates abstractTarget,
    double primaryRoC,
    double secondaryRoC,
    double max_seg_length
    ) :
    vertexArray(vertexArray),
    resolutionScale(resolutionScale),
    game(game),
    secondaryRoC(secondaryRoC),
    CoordinateVariables(
        startCoords,
        {abstractTarget},
        primaryRoC,
        max_seg_length
    ) {
    DebrisSprite::initialization();
}

void DebrisSprite::initialization() {
    CI.evaluateA();
    singlePointGen();
}

void DebrisSprite::update() {
    if (!finished) { //highest possible y value before going offscreen
        if (!pastInterpolations.empty() && (pastInterpolations.size() > MAX_TRAIL_LENGTH ||
            (increment % 2 == 0 && pastInterpolations.size() < MAX_TRAIL_LENGTH))
            ) {
            pastInterpolations.erase(pastInterpolations.begin());
        }
        pastInterpolations.push_back(currentInterpolation);
        setCurrentInterpolation(interpolation());
        if (increment == 60) {
            MAX_RATE_OF_CHANGE = secondaryRoC;
            newTargetPosition(getActualEndCoords());
        }

        game.appendArray(
            getCurrentInterpolation(),
            SpriteSize::DebrisSprite,
            (Utilities::convertToDegrees(getCurrentInterpolation())),
            sf::Color(255, 140, 0)
        );

        int rgb{0};
        sf::Color colorTrail(rgb, rgb, rgb);
        SpriteSize::Dimensions trailSprite{3, 1};
        finished = true;
        if (currentInterpolation.y < 360) {finished = false;}
        for (int i = 0; i < pastInterpolations.size(); i++) {
            if (pastInterpolations[i].y < 360) {
                finished = false;
            }
            colorTrail.r += 5;
            colorTrail.g += 5;
            colorTrail.b += 5;

            game.appendArray(
                pastInterpolations[i],
                trailSprite,
                (Utilities::convertToDegrees(pastInterpolations[i])),
                colorTrail
            );
        }

        increment++;
    } else {
        finished = true;
    }
}

Utilities::Coordinates DebrisSprite::getCurrentInterpolation() const {
    return currentInterpolation;
}

void DebrisSprite::setCurrentInterpolation(Utilities::Coordinates currentInterpolation) {
    this->currentInterpolation = currentInterpolation;
}

bool DebrisSprite::getFinished() const {
    return finished;
}
