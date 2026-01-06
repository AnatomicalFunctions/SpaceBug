/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#ifndef DEBRIS_H
#define DEBRIS_H

#include "../Coordinate Variables Package/CoordinateVariables.h"
#include <SFML/Graphics.hpp>
#include "../Utility Files/SpriteSize.h"
class GameLogic;

/**
 * Defines the animation of a fragment of the CraftExplosion animation
 */
class DebrisSprite : public CoordinateVariables {
private:
    vector<sf::Vertex>& vertexArray;
    const double resolutionScale;
    GameLogic& game;
    vector<Utilities::Coordinates> pastInterpolations;
    Utilities::Coordinates currentInterpolation;
    const double secondaryRoC;
    bool finished{false};
    int increment{0};
    static constexpr int MAX_TRAIL_LENGTH{20};
public:
    /**
     * Default constructor
     */
    DebrisSprite(
        vector<sf::Vertex>& vertexArray,
        const double resolutionScale,
        GameLogic& game,
        Utilities::Coordinates startCoords,
        Utilities::Coordinates abstractTarget,
        double primaryRoC,
        double secondaryRoC,
        double max_seg_length
    );

    /**
     * Initializes the CV of a DebrisSprite
     */
    void initialization() override;

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

#endif //DEBRIS_H
