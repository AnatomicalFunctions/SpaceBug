/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#ifndef CHAFF_H
#define CHAFF_H

#include "../Coordinate Variables Package/CoordinateVariables.h"
#include <SFML/Graphics.hpp>
#include "ChaffSprite.h"
class SpaceCraft;
class GameLogic;

/**
 * Manages several ChaffSprite objects to produce a chaff cloud.
 */
class Chaff {
private:
    vector<sf::Vertex>& vertexArray;
    const double resolutionScale;
    GameLogic& game;
    SpaceCraft& SC;
    vector<ChaffSprite> sprites;
    Utilities::Coordinates center;
    int increment{0};
    bool finished{false};
public:
    /**
     * Standard constructor...
     * Reserves space in the sprites vector and calls update();
     * @param SC : A SpaceCraft reference is needed to render the chaff in the correct location.
     */
    Chaff(
        vector<sf::Vertex>& vertexArray,
        const double resolutionScale,
        GameLogic& game,
        SpaceCraft& SC
    );

    /**
     * Updates object information.
     * Controls chaff animation
     * Sets finished to true once finished.
     */
    void update();

    //Getters
    bool getFinished() const;
    Utilities::Coordinates getCenter() const;
};



#endif //CHAFF_H
