/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#include "Chaff.h"
#include "../SpaceCraft.h"

Chaff::Chaff(
    vector<sf::Vertex>& vertexArray,
    const double resolutionScale,
    GameLogic& game,
    SpaceCraft& SC
    ) :
    vertexArray(vertexArray),
    resolutionScale(resolutionScale),
    game(game),
    SC(SC),
    center(SC.getCurrentInterpolation()
    ) {
    sprites.reserve(24); //Growing the vector destroys the internal pointers, so you need to reserve space.
    update();
}

void Chaff::update() {
    if (sprites.size() == 6) {
        center = SC.getCurrentInterpolation();
    }
    if (increment % 2 == 0 && sprites.size() < 24) {
        sprites.emplace_back(
            vertexArray,
            resolutionScale,
            game,
            SC.getCurrentInterpolation(),
            Utilities::Direction::LEFT_TURN,
            170
        );
        sprites.emplace_back(
            vertexArray,
            resolutionScale,
            game,
            SC.getCurrentInterpolation(),
            Utilities::Direction::RIGHT_TURN,
            170
        );
    }
    finished = true; //tentative
    for (int i = 0; i < sprites.size(); i++) {
        if (!sprites[i].getFinished()) {
            finished = false;
            sprites[i].update();
        }
    }
    increment++;
}

bool Chaff::getFinished() const {
    return finished;
}

Utilities::Coordinates Chaff::getCenter() const {
    return center;
}
