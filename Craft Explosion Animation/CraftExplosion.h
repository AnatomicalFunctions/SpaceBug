/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#ifndef CRAFTEXPLOSION_H
#define CRAFTEXPLOSION_H

#include <SFML/Graphics.hpp>
#include "DebrisSprite.h"
class GameLogic;

/**
 * Defines an explosion animation
 */
class CraftExplosion {
private:
    vector<sf::Vertex>& vertexArray;
    const double resolutionScale;
    GameLogic& game;
    const Utilities::Coordinates startCoords;
    double currentRotation{0};
    vector<DebrisSprite> sprites;
    int increment{0};
    bool finished{false};
public:
    /**
     * Standard constructor...
     * Reserves space in the sprites vector and calls update();
     */
    CraftExplosion(
        vector<sf::Vertex>& vertexArray,
        const double resolutionScale,
        GameLogic& game,
        Utilities::Coordinates startCoords,
        double rotation
    );

    /**
     * Updates object information.
     * Controls CraftExplosion animation
     * Sets finished to true once finished.
     */
    void update();

    void generateCircle(
        Utilities::Coordinates ex_coords,
        sf::Color color,
        double size_scale
    );

    void appendTriangleArray(
        Utilities::Coordinates zero_point,
        Utilities::Coordinates point_one,
        Utilities::Coordinates point_two,
        Utilities::Coordinates point_three,
        Utilities::SinAndCos sin_and_cos,
        sf::Color color
    );

    //Getters
    bool getFinished() const;
};



#endif //CRAFTEXPLOSION_H
