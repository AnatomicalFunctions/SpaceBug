/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#ifndef MISSILE_H
#define MISSILE_H

#include "Utility Files/SpriteSize.h"
#include "Utility Files/Utilities.h"
#include "Craft Explosion Animation/CraftExplosion.h"
#include "Coordinate Variables Package/CoordinateVariables.h"
#include <SFML/Graphics.hpp>
class GameLogic;

/**
 * Defines the behavior of a missile
 */
class Missile : public CoordinateVariables {
private:
    vector<sf::Vertex>& vertexArray;
    const double resolutionScale;
    GameLogic& game;
    Utilities::Coordinates currentInterpolation;
    vector<Utilities::Coordinates> pastInterpolations;
    const int MAX_INCREMENT;
    int increment{0};
    double rotation{0};
    bool finished_update_head{false};
    bool finished_update_tail{false};
    bool finished_animation{false};
    bool finished{false};
    vector<unique_ptr<CraftExplosion>> explosion;
    static constexpr int MAX_TRAIL_LENGTH{20};
public:
    /**
     * Calls constructor in CoordinateVariables
     * Initializes missileShape
     */
    Missile(
        vector<sf::Vertex>& vertexArray,
        const double resolutionScale,
        GameLogic& game,
        Utilities::CVBundle cv_bundle,
        int MAX_INCREMENT
    );

    /**
     * Copy constructor implemented for use in Utilities::goForChaff()
     * Needs special code for Missile so it doesn't attempt to copy the explosions vector.
     */
    Missile(const Missile& other);

    /**
     * Initializes the CV of a Missile object
     */
    void initialization() override;

    /**
     * Updates object information
     * Requires: currentInterpolation, missileShape, resolutionScale, Utilities::convertToDegrees()
     * Modifies: currentInterpolation
     */
    void update();

    /**
     * For when the Missile has found a target
     */
    void impact();

    /**
     * For when the Missile has hit the border
     */
    void impactBorderUpDown();

    /**
     * For when the Missile has hit the border
     */
    void impactBorderLeftRight();

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



#endif //MISSILE_H
