/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#ifndef BOSSONE_H
#define BOSSONE_H

#include "SpaceCraft.h"
class RoundOne;

class BossOne : public SpaceCraft {
private:
    RoundOne& RO;
    static constexpr int MAX_TRAIL_LENGTH{30};
    vector<unique_ptr<Missile>> missiles;
    vector<double> missile_queue;
    int explosion_rotation{0};
    int hit_increment{0};
    int explosion_increment{0};
    int chaff_increment{MAX_CHAFF_INCREMENT};
    static constexpr int MAX_CHAFF_INCREMENT{50};
    bool hit{false};
    int health{MAX_HEALTH};
    static constexpr int MAX_HEALTH{15};
public:
    BossOne(
        vector<sf::Vertex>& vertexArray,
        const double resolutionScale,
        GameLogic& game,
        RoundOne& RO,
        Utilities::Coordinates startCoords,
        double ROC_ONE,
        double ROC_TWO,
        double MAX_SEGMENT_LENGTH
    );

    void update() override;

    void render() override;

    void impact() override;

    void fireUpdate();

    //Even numbers only
    void fireMissile(int num_missiles);
    void fireMissile();

    void reset(
        Utilities::Coordinates startCoords
    ) override;
};



#endif //BOSSONE_H
