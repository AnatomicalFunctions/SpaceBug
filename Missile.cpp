/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#include "Missile.h"
#include "CollisionDetection.h"
#include "Game Logic Package/GameLogic.h"

using namespace std;

Missile::Missile(
    vector<sf::Vertex>& vertexArray,
    const double resolutionScale,
    GameLogic& game,
    Utilities::CVBundle cv_bundle,
    int MAX_INCREMENT
    ) :
    vertexArray(vertexArray),
    resolutionScale(resolutionScale),
    game(game),
    CoordinateVariables(cv_bundle),
    MAX_INCREMENT(MAX_INCREMENT
    ) {
    Missile::initialization();
}

Missile::Missile(
    const Missile& other
    ) :
    vertexArray(other.vertexArray),
    resolutionScale(other.resolutionScale),
    game(other.game),
    currentInterpolation(other.currentInterpolation),
    MAX_INCREMENT(other.MAX_INCREMENT),
    increment(increment),
    finished_update_head(finished_update_head),
    finished_update_tail(finished_update_tail),
    finished_animation(finished_animation),
    finished(finished),
    //explosions isn't copied because they are unique_ptrs
    CoordinateVariables(other) {
};

void Missile::initialization() {
    CI.evaluateA();
    singlePointGen();
}

void Missile::update() {
    if (!finished_update_head) {
        if (currentInterpolation.x > SpriteSize::Resolution.x || currentInterpolation.x < 0) {
            impactBorderLeftRight();
        } else if (currentInterpolation.y > SpriteSize::Resolution.y || currentInterpolation.y < 0) {
            impactBorderUpDown();
        } else if (increment >= MAX_INCREMENT) {
            impact();
        }

        if (!pastInterpolations.empty() && (pastInterpolations.size() > MAX_TRAIL_LENGTH ||
            (increment % 2 == 0 && pastInterpolations.size() < MAX_TRAIL_LENGTH))
            ) {
            pastInterpolations.erase(pastInterpolations.begin());
        }
        pastInterpolations.push_back(currentInterpolation);
        setCurrentInterpolation(interpolation());

        rotation = Utilities::convertToDegrees(getCurrentInterpolation());
        Utilities::SinAndCos sin_and_cos(rotation);
        CollisionDetection::PlainCoordinates c_i_1{
            CollisionDetection::preCalculatedRotation(
                {getCurrentInterpolation().x, getCurrentInterpolation().y - (SpriteSize::Missile.y / 2.0)},
                getCurrentInterpolation(),
                sin_and_cos
            )};
        CollisionDetection::PlainCoordinates c_i_2{
            CollisionDetection::preCalculatedRotation(
                {getCurrentInterpolation().x, (getCurrentInterpolation().y + SpriteSize::Missile.y / 2.0)},
                getCurrentInterpolation(),
                sin_and_cos
            )};
        CollisionDetection::PlainCoordinates c_i_3{
            CollisionDetection::preCalculatedRotation(
                {getCurrentInterpolation().x + SpriteSize::Missile.x, getCurrentInterpolation().y},
                getCurrentInterpolation(),
                sin_and_cos
            )};

        sf::Color color(200, 60, 200);
        //Calculates the increments needed to reach the desired end-color:
        //255, 0, 0
        //Occurance is there to smooth out integer division
        int occurances{10};
        for (int i = 0; i < increment / occurances; i++) {
            color.r += (55 / (MAX_INCREMENT / occurances));
            color.g -= (60 / (MAX_INCREMENT / occurances));
            color.b -= (200 / (MAX_INCREMENT / occurances));
        }

        vertexArray.push_back(sf::Vertex({float(c_i_1.x * resolutionScale), float(c_i_1.y * resolutionScale)}, color));
        vertexArray.push_back(sf::Vertex({float(c_i_2.x * resolutionScale), float(c_i_2.y * resolutionScale)}, color));
        vertexArray.push_back(sf::Vertex({float(c_i_3.x * resolutionScale), float(c_i_3.y * resolutionScale)}, color));
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
        SpriteSize::Dimensions trailSprite{3, 1};
        for (int i = 0; i < pastInterpolations.size(); i++) {

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

    if (finished_animation &&
        finished_update_head &&
        finished_update_tail
        ) {
        finished = true;
    }

    increment++;
}

void Missile::impact() {
    if (!finished_update_head) {
        finished_update_head = true;
        explosion.push_back(make_unique<CraftExplosion>(
            vertexArray, resolutionScale, game,
            getCurrentInterpolation(),
            Utilities::convertToDegrees(getCurrentInterpolation())));
        setCurrentInterpolation(Utilities::Coordinates{});
    }
}

void Missile::impactBorderUpDown() {
    if (!finished_update_head) {
        finished_update_head = true;
        explosion.push_back(make_unique<CraftExplosion>(
            vertexArray, resolutionScale, game,
            getCurrentInterpolation(),
            -Utilities::convertToDegrees(getCurrentInterpolation())));
        setCurrentInterpolation(Utilities::Coordinates{});
    }
}

void Missile::impactBorderLeftRight() {
    if (!finished_update_head) {
        finished_update_head = true;
        explosion.push_back(make_unique<CraftExplosion>(
            vertexArray, resolutionScale, game,
            getCurrentInterpolation(),
            -Utilities::convertToDegrees(getCurrentInterpolation()) + 180));
        setCurrentInterpolation(Utilities::Coordinates{});
    }
}

//pastInterpolations is not reset, so it leaves a nice trace behind to fade away...
void Missile::reset(
    Utilities::Coordinates startCoords,
    Utilities::Coordinates endCoords
    ) {
    setCurrentInterpolation(Utilities::Coordinates{});
    increment = 0;
    finished_update_head = false;
    finished_update_tail = false;
    finished_animation = false;
    finished = false;
    explosion.clear();
    CoordinateVariables::reset(startCoords, endCoords);
}

Utilities::Coordinates Missile::getCurrentInterpolation() const {
    return currentInterpolation;
}

bool Missile::getFinished() const {
    return finished;
}

double Missile::getRotation() const {
    return rotation;
}

void Missile::setCurrentInterpolation(Utilities::Coordinates currentInterpolation) {
    this->currentInterpolation = currentInterpolation;
}
