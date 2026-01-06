/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#ifndef GUNFIRE_H
#define GUNFIRE_H

#include "Utility Files/SpriteSize.h"
#include "Utility Files/Utilities.h"
#include "Craft Explosion Animation/CraftExplosion.h"
#include "Coordinate Variables Package/CoordinateVariables.h"
#include <SFML/Graphics.hpp>
class GameLogic;

/**
 * Defines the behavior of a Gunfire
 */
class Gunfire : public CoordinateVariables {
private:
    vector<sf::Vertex>& vertexArray;
    const double resolutionScale;
    GameLogic& game;
    Utilities::Coordinates currentInterpolation;
    vector<Utilities::Coordinates> pastInterpolations;
    int increment{0};
    double rotation{0};
    bool finished_update_head{false};
    bool finished_update_tail{false};
    bool finished{false};
    static constexpr int MAX_TRAIL_LENGTH{3};
public:
    /**
     * Calls constructor in CoordinateVariables
     * Initializes GunfireShape
     */
    Gunfire(
        vector<sf::Vertex>& vertexArray,
        const double resolutionScale,
        GameLogic& game,
        Utilities::Coordinates startCoords,
        int spread
    );

    /**
     * Copy constructor implemented for use in Utilities::goForChaff()
     * Needs special code for Gunfire so it doesn't attempt to copy the explosions vector.
     */
    Gunfire(const Gunfire& other);

    /**
     * Initializes the CV of a Gunfire object
     */
    void initialization() override;

    /**
     * Updates object information
     * Requires: currentInterpolation, GunfireShape, resolutionScale, Utilities::convertToDegrees()
     * Modifies: currentInterpolation
     */
    void update();

    /**
     * For when the Gunfire has found a target
     */
    void impact();

    void reset(
        Utilities::Coordinates startCoords,
        Utilities::Coordinates endCoords
    ) override;

    //Getters
    Utilities::Coordinates getCurrentInterpolation() const;
    bool getFinished() const;
    double getRotation() const;

    //Setters
    void setCurrentInterpolation(Utilities::Coordinates currentInterpolation);
};


#endif //GUNFIRE_H
