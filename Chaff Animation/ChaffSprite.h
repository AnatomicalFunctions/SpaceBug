/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#ifndef CHAFFSPRITE_H
#define CHAFFSPRITE_H

#include "../Coordinate Variables Package/CoordinateVariables.h"
#include <SFML/Graphics.hpp>
class GameLogic;

/**
 * Defines the behavior of a chaff sprite
 */
class ChaffSprite : public CoordinateVariables {
private:
    vector<sf::Vertex>& vertexArray;
    const double resolutionScale;
    GameLogic& game;
    Utilities::Coordinates currentInterpolation;
    vector<Utilities::Coordinates> pastInterpolations;
    int increment{0};
    const int MAX_INCREMENT;
    const double MAX_ARC_LENGTH{10};
    static constexpr int MAX_TRAIL_LENGTH{30};
    Utilities::Direction direction_modifier;
    bool finished_head{false};
    bool finished_tail{false};
    bool finished{false};
public:
    /**
     *
     */
    ChaffSprite(
        vector<sf::Vertex>& vertexArray,
        const double resolutionScale,
        GameLogic& game,
        Utilities::Coordinates startCoords,
        Utilities::Direction direction_modifier,
        int MAX_INCREMENT
    );

    /**
     * Sets "a" to the desired value and calls rocArcGenRecursion(MAX_ARC_LENGTH) once to populate vCoords
     */
    void initialization() override;

    /**
     * Specifies arcGen behavior when called on a ChaffSprite object
     * Requires: rocArcGenRecursion(), MAX_ARC_LENGTH
     */
    void arcGenCall() override;

    /**
     * Updates object information
     */
    void update();

    //Getters
    Utilities::Coordinates getCurrentInterpolation() const;
    bool getFinished() const;

    //Setters
    void setCurrentInterpolation(Utilities::Coordinates currentInterpolation);
};



#endif //CHAFFSPRITE_H
