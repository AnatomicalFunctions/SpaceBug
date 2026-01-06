/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#include "ChaffSprite.h"
#include <iostream>
#include "../CollisionDetection.h"
#include "../Game Logic Package/GameLogic.h"
#include "../Utility Files/SpriteSize.h"
using namespace std;

ChaffSprite::ChaffSprite(
    vector<sf::Vertex>& vertexArray,
    const double resolutionScale,
    GameLogic& game,
    Utilities::Coordinates startCoords,
    Utilities::Direction direction_modifier,
    int MAX_INCREMENT
    ) :
    vertexArray(vertexArray),
    resolutionScale(resolutionScale),
    game(game),
    direction_modifier(direction_modifier),
    MAX_INCREMENT(MAX_INCREMENT),
    CoordinateVariables(
        startCoords,
        0.01,
        0.7
    ) {
    ChaffSprite::initialization();
}

void ChaffSprite::initialization() {
    setA(int(direction_modifier) * 0.01);
    rocArcGenRecursion(MAX_ARC_LENGTH);
}

void ChaffSprite::arcGenCall() {
    rocArcGenRecursion(MAX_ARC_LENGTH);
}

void ChaffSprite::update() {
    if (increment > MAX_INCREMENT) {
        finished_head = true;
    }
    if (!finished_head) {
        if (!pastInterpolations.empty() && (pastInterpolations.size() > MAX_TRAIL_LENGTH ||
            (increment % 20 == 0 && pastInterpolations.size() < MAX_TRAIL_LENGTH))
            ) {
            pastInterpolations.erase(pastInterpolations.begin());
        }
        if (increment % int((-1 * 10 * MAX_SEGMENT_LENGTH) + 8) == 0) {
            pastInterpolations.push_back(currentInterpolation);
        }
        setCurrentInterpolation(interpolation());

        game.appendArray(
            getCurrentInterpolation(),
            SpriteSize::ChaffSprite,
            (Utilities::convertToDegrees(getCurrentInterpolation())),
            sf::Color(255, 255, 255)
        );

        if (increment % 2 == 0 && MAX_SEGMENT_LENGTH > 0.35) {
            MAX_SEGMENT_LENGTH -= 0.02;
        } else if (increment % 2 == 0 && MAX_SEGMENT_LENGTH > 0.1) {
            MAX_SEGMENT_LENGTH -= 0.01;
        }
    }

    if (!finished_tail) {
        if (finished_head) {
            if (pastInterpolations.empty()) {
                finished_tail = true;
            } else if (!pastInterpolations.empty() && increment % 2 == 0) {
                pastInterpolations.erase(pastInterpolations.begin());
            }
        }

        sf::Color color(0, 0, 0);
        SpriteSize::Dimensions trailSprite{3, 1};
        for (int i = 0; i < pastInterpolations.size(); i++) {

            color.r += 8;
            color.g += 8;
            color.b += 8;

            game.appendArray(
                pastInterpolations[i],
                trailSprite,
                (Utilities::convertToDegrees(pastInterpolations[i])),
                color
            );
        }
    }

    if (finished_head && finished_tail) {
        finished = true;
    }

    increment++;
}

Utilities::Coordinates ChaffSprite::getCurrentInterpolation() const {
    return currentInterpolation;
}

bool ChaffSprite::getFinished() const {
    return finished;
}

void ChaffSprite::setCurrentInterpolation(Utilities::Coordinates currentInterpolation) {
    this->currentInterpolation = currentInterpolation;
}
