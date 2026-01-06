/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#include "Gunfire.h"
#include "CollisionDetection.h"
#include "Game Logic Package/GameLogic.h"

using namespace std;

Gunfire::Gunfire(
    vector<sf::Vertex>& vertexArray,
    const double resolutionScale,
    GameLogic& game,
    Utilities::Coordinates startCoords,
    int spread
    ) :
    vertexArray(vertexArray),
    resolutionScale(resolutionScale),
    game(game),
    rotation(Utilities::convertToDegrees(startCoords) + spread),
    CoordinateVariables(
        startCoords,
        CollisionDetection::rotateObject(
            {
                startCoords.x + 10000,
                ((startCoords.x + 10000) * startCoords.slope) +
                    CollisionDetection::ObjectPoints::calculateIntercept(
                        {startCoords.x, startCoords.y},
                        startCoords.slope
                    )
            },
            startCoords,
            Utilities::convertToDegrees(startCoords) + spread
        ),
        0.0094375,
        6
    ) {
    Gunfire::initialization();
}

Gunfire::Gunfire(
    const Gunfire& other
    ) :
    vertexArray(other.vertexArray),
    resolutionScale(other.resolutionScale),
    game(other.game),
    currentInterpolation(other.currentInterpolation),
    increment(increment),
    finished_update_head(finished_update_head),
    finished_update_tail(finished_update_tail),
    finished(finished),
    //explosions isn't copied because they are unique_ptrs
    CoordinateVariables(other) {
};

void Gunfire::initialization() {
    CI.evaluateA();
    singlePointGen();
}

void Gunfire::update() {
    if (!finished_update_head) {
        if (currentInterpolation.x > SpriteSize::Resolution.x || currentInterpolation.x < 0 ||
            currentInterpolation.y > SpriteSize::Resolution.y || currentInterpolation.y < 0
        ) {
            impact();
        }

        if (!pastInterpolations.empty() && (pastInterpolations.size() > MAX_TRAIL_LENGTH ||
            (increment % 2 == 0 && pastInterpolations.size() < MAX_TRAIL_LENGTH))
            ) {
            pastInterpolations.erase(pastInterpolations.begin());
        }
        pastInterpolations.push_back(currentInterpolation);
        setCurrentInterpolation(interpolation());

        sf::Color color(255, 0, 0);
        game.appendArray(
            getCurrentInterpolation(),
            SpriteSize::Gunfire,
            rotation,
            color
        );
    }

    if (!finished_update_tail) {
        if (finished_update_head) {
            if (pastInterpolations.empty()) {
                finished_update_tail = true;
            } else if (!pastInterpolations.empty() && increment % 2 == 0) {
                pastInterpolations.erase(pastInterpolations.begin());
            }
        }

        int rgb{0};
        sf::Color colorTrail(rgb, rgb, rgb);
        SpriteSize::Dimensions trailSprite{1, 1};
        for (int i = 0; i < pastInterpolations.size(); i++) {

            colorTrail.r += 80;

            game.appendArray(
                pastInterpolations[i],
                trailSprite,
                (Utilities::convertToDegrees(pastInterpolations[i])),
                colorTrail
            );
        }
    }

    if (finished_update_head &&
        finished_update_tail
        ) {
        finished = true;
    }

    increment++;
}

void Gunfire::impact() {
    if (!finished_update_head) {
        finished_update_head = true;
        setCurrentInterpolation(Utilities::Coordinates{-50, -50});
    }
}

//pastInterpolations is not reset, so it leaves a nice trace behind to fade away...
void Gunfire::reset(
    Utilities::Coordinates startCoords,
    Utilities::Coordinates endCoords
    ) {
    setCurrentInterpolation(Utilities::Coordinates{});
    increment = 0;
    finished_update_head = false;
    finished_update_tail = false;
    finished = false;
    CoordinateVariables::reset(startCoords, endCoords);
}

Utilities::Coordinates Gunfire::getCurrentInterpolation() const {
    return currentInterpolation;
}

bool Gunfire::getFinished() const {
    return finished;
}

double Gunfire::getRotation() const {
    return rotation;
}

void Gunfire::setCurrentInterpolation(Utilities::Coordinates currentInterpolation) {
    this->currentInterpolation = currentInterpolation;
}
