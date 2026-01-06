/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#ifndef ROUNDONE_H
#define ROUNDONE_H

#include "../Craft Explosion Animation/CraftExplosion.h"
#include "../SpaceCraft.h"
#include "../BossOne.h"
#include "../Utility Files/Utilities.h"
#include "../Missile.h"
class GameLogic;

/**
 *
 */
class RoundOne {
private:
    vector<sf::Vertex>& vertexArray;
    const double resolutionScale;
    GameLogic& game;
    SpaceCraft SC;
    BossOne BO;
    vector<Missile> missileObjects;
    vector<unique_ptr<CraftExplosion>> explosion;
    int chaff_increment{MAX_CHAFF_INCREMENT};
    static constexpr int MAX_CHAFF_INCREMENT{50};
    bool finished_head{false};
public:
    RoundOne(vector<sf::Vertex>& vertexArray, const double resolutionScale, GameLogic& game);

    /**
     * Generated a chaff object within SC
     */
    void chaff();

    void gunfire();

    void resetGunfireIncrement();

    /**
     * Generates a CraftExplosion object.
     */
    void explode();

    /**
     * Updates RoundOne objects
     */
    void update();

    /**
     * Resets the position of the Missile objects.
     */
    void reset();

    SpaceCraft& getSpaceCraft();
};



#endif //ROUNDONE_H
