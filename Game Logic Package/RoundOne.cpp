/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#include "RoundOne.h"

#include "../CollisionDetection.h"
#include "../Missile.h"
#include "../Utility Files/StaticTranslationHandling.h"

RoundOne::RoundOne(
    vector<sf::Vertex>& vertexArray,
    const double resolutionScale,
    GameLogic& game
    ) :
    vertexArray(vertexArray),
    resolutionScale(resolutionScale),
    game(game),
    SC(
        vertexArray,
        resolutionScale,
        game,
        Utilities::Coordinates{10, 90, 0, 0, Utilities::Quadrants::UPRIGHT},
        0.00421875,
        0.0084375,
        2.5
    ),
    BO(
        vertexArray,
        resolutionScale,
        game,
        *this,
        Utilities::Coordinates{630, 200, 0, 0, Utilities::Quadrants::DOWNLEFT},
        0.00421875,
        0.0084375,
        1.25
    ) {
    missileObjects.reserve(24); //Growing the vector destroys the internal pointers, so you need to reserve space.
    int k = 0;
    for (int i = 0; i < 0; i++) {
        double sp = 0.25;
        for (int j = 0; j < 3; j++) {
            Utilities::CVBundle cv_bundle = {
                Utilities::Coordinates{160, 90, 0, sp, Utilities::Quadrants(i)},
                Utilities::Coordinates{400, 90},
                0.0094375,
                2.8
            };
            missileObjects.emplace_back(vertexArray, resolutionScale, game, cv_bundle, 500);
            sp -= 0.25;
            k++;
        }
    }
}

void RoundOne::chaff() {
    SC.chaffGen();
}

void RoundOne::gunfire() {
    SC.gunfireGen();
}

void RoundOne::resetGunfireIncrement() {
    SC.resetGunfireIncrement();
}

void RoundOne::explode() {
    explosion.push_back(make_unique<CraftExplosion>(
        vertexArray, resolutionScale, game,
        SC.getCurrentInterpolation(),
        Utilities::convertToDegrees(SC.getCurrentInterpolation())));
}

void RoundOne::update() {
    if (chaff_increment < MAX_CHAFF_INCREMENT) {
        chaff_increment++;
    }
    SC.update();
    BO.update();
    // for (int i = 0; i < missileObjects.size(); i++) {
    //     if (!finished_head) {
    //         if (Utilities::goForChaff(missileObjects[i], SC)) {
    //             missileObjects[i].newTargetPosition(SC.getChaffCenter());
    //             chaff_increment = 0;
    //         } else if (
    //             chaff_increment == MAX_CHAFF_INCREMENT ||
    //             Utilities::goForTarget(missileObjects[i], SC.getCurrentInterpolation())
    //             ) {
    //             missileObjects[i].newTargetPosition(SC.getCurrentInterpolation());
    //         }
    //
    //         //Coords are shifted downward to interface with CollisionDetection, which draws from a corner.
    //         Utilities::Coordinates s_craft_i = SC.getCurrentInterpolation();
    //         Utilities::Coordinates missile_i = missileObjects[i].getCurrentInterpolation();
    //         s_craft_i.y -= (SpriteSize::SpaceCraft.y / 2);
    //         missile_i.y -= (SpriteSize::Missile.y / 2);
    //
    //         for (int j = 0; j < SC.getGunfire().size(); j++) {
    //             Utilities::Coordinates gunfire_i = SC.getGunfire()[j]->getCurrentInterpolation();
    //             gunfire_i.y -= (SpriteSize::Gunfire.y / 2);
    //             if (CollisionDetection::checkCollision(
    //             gunfire_i,
    //             SC.getGunfire()[j]->getCurrentInterpolation(),
    //             SpriteSize::Gunfire,
    //             SC.getGunfire()[j]->getRotation(),
    //             missile_i,
    //             missileObjects[i].getCurrentInterpolation(),
    //             SpriteSize::Missile,
    //             missileObjects[i].getRotation()
    //             )) {
    //                 SC.getGunfire()[j]->impact();
    //                 missileObjects[i].impact();
    //             }
    //         }
    //
    //         if (CollisionDetection::checkCollision(
    //             s_craft_i,
    //             SC.getCurrentInterpolation(),
    //             SpriteSize::SpaceCraft,
    //             SC.getRotation(),
    //             missile_i,
    //             missileObjects[i].getCurrentInterpolation(),
    //             SpriteSize::Missile,
    //             missileObjects[i].getRotation()
    //         )) {
    //             finished_head = true;
    //
    //             SC.impact();
    //             missileObjects[i].impact();
    //
    //             //Trails the remaining missiles off into the distance.
    //             for (int i = 0; i < missileObjects.size(); i++) {
    //                 Utilities::Coordinates c_i = missileObjects[i].getCurrentInterpolation();
    //                 double b{CollisionDetection::ObjectPoints::calculateIntercept({c_i.x, c_i.y}, c_i.slope)};
    //                 c_i.x += 10000;
    //                 c_i.y = (c_i.x * c_i.slope) + b;
    //                 missileObjects[i].newTargetPosition(CollisionDetection::rotateObject(
    //                     c_i,
    //                     missileObjects[i].getCurrentInterpolation(),
    //                     missileObjects[i].getRotation()
    //                 ));
    //             }
    //         }
    //     }
    //
    //     missileObjects[i].update();
    // }
    for (int i = 0; i < explosion.size(); i++) {
        explosion[i]->update();
        if (explosion[i]->getFinished() == true) {
            explosion.erase(explosion.begin() + i);
            i--;
        }
    }
}

void RoundOne::reset() {
    finished_head = false;
    chaff_increment = MAX_CHAFF_INCREMENT;
    SC.reset(Utilities::Coordinates{10, 90, 0, 0, Utilities::Quadrants::UPRIGHT});
    BO.reset(Utilities::Coordinates{630, 200, 0, 0, Utilities::Quadrants::DOWNLEFT});
    int k = 0;
    for (int i = 0; i < 0; i++) {
        double sp = 0.25;
        for (int j = 0; j < 3; j++) {
            missileObjects[k].reset(Utilities::Coordinates{160, 90, 0, sp, Utilities::Quadrants(i)},
                                    Utilities::Coordinates{SC.getCurrentInterpolation().x, SC.getCurrentInterpolation().y});
            sp -= 0.25;
            k++;
        }
    }
}

SpaceCraft& RoundOne::getSpaceCraft() {
    return SC;
}
