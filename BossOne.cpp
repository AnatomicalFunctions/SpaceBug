/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#include "BossOne.h"
#include "CollisionDetection.h"
#include "Game Logic Package/GameLogic.h"

BossOne::BossOne(
    vector<sf::Vertex>& vertexArray,
    const double resolutionScale,
    GameLogic& game,
    RoundOne& RO,
    Utilities::Coordinates startCoords,
    double ROC_ONE,
    double ROC_TWO,
    double MAX_SEGMENT_LENGTH
    ) :
    RO(RO),
    SpaceCraft(
        vertexArray,
        resolutionScale,
        game,
        startCoords,
        ROC_ONE,
        ROC_TWO,
        MAX_SEGMENT_LENGTH
    ) {
}

void BossOne::update() {

    if (health > 0) {
        sf::Color health_color(0, 200, 0);
        for (int i = MAX_HEALTH; i > health; i--) {
            health_color.r += 13;
            health_color.g -= 13;
        }
        game.appendArray(
            {245, 10},
            SpriteSize::Dimensions{SpriteSize::Healthbar.x - (150 - (health * 10)), SpriteSize::Healthbar.y},
            0,
            health_color
        );
    }

    if (chaff_increment < MAX_CHAFF_INCREMENT) {
        chaff_increment++;
    }

    setCurrentInterpolation(interpolation());
    rotation = Utilities::convertToDegrees(currentInterpolation);

    if (!finished_head) {
        if (health <= 0) {
            impact();
        }

        if (!RO.getSpaceCraft().getFinished()) {
            if (increment > 100) {
                if (increment % 1200 == 0) {
                    fireMissile(8);
                } else if (increment % 600 == 0) {
                    fireMissile(4);
                } else if (increment % 350 == 0) {
                    fireMissile();
                }
            } else if (increment == 50) {
                fireMissile(4);
            }
        }

        if (!pastInterpolations.empty() && (pastInterpolations.size() > MAX_TRAIL_LENGTH ||
            (increment % 2 == 0 && pastInterpolations.size() < MAX_TRAIL_LENGTH))
            ) {
            pastInterpolations.erase(pastInterpolations.begin());
        }
        pastInterpolations.push_back(currentInterpolation);

        Utilities::Coordinates s_craft_i = RO.getSpaceCraft().getCurrentInterpolation();
        s_craft_i.y -= (SpriteSize::SpaceCraft.y / 2);

        for (int i = 0; i < missiles.size(); i++) {
            if (Utilities::goForChaff(*missiles[i], RO.getSpaceCraft())) {
                missiles[i]->newTargetPosition(RO.getSpaceCraft().getChaffCenter());
                chaff_increment = 0;
            } else if (
                !RO.getSpaceCraft().getFinished() &&
                (chaff_increment == MAX_CHAFF_INCREMENT ||
                Utilities::goForTarget(*missiles[i], RO.getSpaceCraft().getCurrentInterpolation()))
                ) {
                missiles[i]->newTargetPosition(RO.getSpaceCraft().getCurrentInterpolation());
            } else if (RO.getSpaceCraft().getFinished()) {
                Utilities::Coordinates c_i = missiles[i]->getCurrentInterpolation();
                double b{CollisionDetection::ObjectPoints::calculateIntercept({c_i.x, c_i.y}, c_i.slope)};
                c_i.x += 10000;
                c_i.y = (c_i.x * c_i.slope) + b;
                missiles[i]->newTargetPosition(CollisionDetection::rotateObject(
                    c_i,
                    missiles[i]->getCurrentInterpolation(),
                    missiles[i]->getRotation()
                ));
            }

            //Coords are shifted downward to interface with CollisionDetection, which draws from a corner.
            Utilities::Coordinates missile_i = missiles[i]->getCurrentInterpolation();
            missile_i.y -= (SpriteSize::Missile.y / 2);

            //Collision Check: Gunfire x Missiles
            for (int j = 0; j < RO.getSpaceCraft().getGunfire().size(); j++) {
                Utilities::Coordinates gunfire_i = RO.getSpaceCraft().getGunfire()[j]->getCurrentInterpolation();
                gunfire_i.y -= (SpriteSize::Gunfire.y / 2);
                if (CollisionDetection::checkCollision(
                gunfire_i,
                RO.getSpaceCraft().getGunfire()[j]->getCurrentInterpolation(),
                SpriteSize::Gunfire,
                RO.getSpaceCraft().getGunfire()[j]->getRotation(),
                missile_i,
                missiles[i]->getCurrentInterpolation(),
                SpriteSize::Missile,
                missiles[i]->getRotation()
                )) {
                    RO.getSpaceCraft().getGunfire()[j]->impact();
                    missiles[i]->impact();
                }
            }

            //Collision Check: SpaceCraft x Missiles
            if (CollisionDetection::checkCollision(
                s_craft_i,
                RO.getSpaceCraft().getCurrentInterpolation(),
                SpriteSize::SpaceCraft,
                RO.getSpaceCraft().getRotation(),
                missile_i,
                missiles[i]->getCurrentInterpolation(),
                SpriteSize::Missile,
                missiles[i]->getRotation()
            )) {
                RO.getSpaceCraft().impact();
                missiles[i]->impact();
            }

            missiles[i]->update();
            if (missiles[i]->getFinished()) {missiles.erase(missiles.begin() + i); i--;}
        }

        Utilities::Coordinates boss_i = currentInterpolation;
        boss_i.y -= (SpriteSize::BossOne.y / 2);
        boss_i.x -= (SpriteSize::BossOne.x / 2);

        for (int j = 0; j < RO.getSpaceCraft().getGunfire().size(); j++) {
            Utilities::Coordinates gunfire_i = RO.getSpaceCraft().getGunfire()[j]->getCurrentInterpolation();
            gunfire_i.y -= (SpriteSize::Gunfire.y / 2);

            if (CollisionDetection::checkCollision(
                gunfire_i,
                RO.getSpaceCraft().getGunfire()[j]->getCurrentInterpolation(),
                SpriteSize::Gunfire,
                RO.getSpaceCraft().getGunfire()[j]->getRotation(),
                boss_i,
                currentInterpolation,
                SpriteSize::BossOne,
                rotation
            )) {
                RO.getSpaceCraft().getGunfire()[j]->impact();
                hit = true;
                health--;
            }
        }

        //Collision Check: SpaceCraft x Boss
        if (CollisionDetection::checkCollision(
            s_craft_i,
            RO.getSpaceCraft().getCurrentInterpolation(),
            SpriteSize::SpaceCraft,
            RO.getSpaceCraft().getRotation(),
            boss_i,
            currentInterpolation,
            SpriteSize::BossOne,
            rotation
        )) {
            RO.getSpaceCraft().impact();
            hit = true;
            health -= 3;
        }

        //Movement Control:
        int border_approach_x{150};
        int border_approach_y{100};

        if (currentInterpolation.x > SpriteSize::Resolution.x - border_approach_x) {
            if ((rotation >= 0 && rotation <= 90) || (rotation <= -270 && rotation >= -360)) {
                input(DEPRESS_LEFT);
            } else if ((rotation >= 270 && rotation <= 360) || (rotation <= 0 && rotation >= -90)) {
                input(DEPRESS_RIGHT);
            }
        } else if (currentInterpolation.x < border_approach_x) {
            if ((rotation >= 90 && rotation <= 180 ) || (rotation <= -180 && rotation >= -270)) {
                input(DEPRESS_RIGHT);
            } else if ((rotation >= -180 && rotation <= -90) || (rotation <= 270 && rotation >= 180)) {
                input(DEPRESS_LEFT);
            }
        } else if (currentInterpolation.y > SpriteSize::Resolution.y - border_approach_y) {
            if ((rotation >= 0 && rotation <= 90) || (rotation <= -270 && rotation >= -360)) {
                input(DEPRESS_RIGHT);
            } else if ((rotation >= 90 && rotation <= 180 ) || (rotation <= -180 && rotation >= -270)) {
                input(DEPRESS_LEFT);
            }
        } else if (currentInterpolation.y < border_approach_y) {
            if ((rotation >= 270 && rotation <= 360) || (rotation <= 0 && rotation >= -90)) {
                input(DEPRESS_LEFT);
            } else if ((rotation >= -180 && rotation <= -90) || (rotation <= 270 && rotation >= 180)) {
                input(DEPRESS_RIGHT);
            }
        } else {
            if (right_depressed) {
                input(RELEASE_RIGHT);
            } else if (left_depressed) {
                input(RELEASE_LEFT);
            }
        }

        fireUpdate();
    } else {
        if (right_depressed) {
            input(RELEASE_RIGHT);
        } else if (left_depressed) {
            input(RELEASE_LEFT);
        }

        for (int i = 0; i < missiles.size(); i++) {
            missiles[i]->impact();
            missiles[i]->update();
            if (missiles[i]->getFinished()) {missiles.erase(missiles.begin() + i); i--;}
        }

        //Finale!
        if (explosion_increment == 60) {
            explosion.push_back(make_unique<CraftExplosion>(
                vertexArray, resolutionScale, game,
                getCurrentInterpolation(),
                -120
            ));
        } else if (explosion_increment == 120) {
            explosion.push_back(make_unique<CraftExplosion>(
                vertexArray, resolutionScale, game,
                getCurrentInterpolation(),
                70
            ));
        } else if (explosion_increment > 180 && explosion_increment < 260 && explosion_increment % 4 == 0) {
            for (int i = 0; i < 4; i++) {
                explosion.push_back(make_unique<CraftExplosion>(
                vertexArray, resolutionScale, game,
                getCurrentInterpolation(),
                explosion_rotation
                ));
                explosion_rotation -= 22.5;
                if (explosion_rotation <= -360) {explosion_rotation += 360;}
            }
        }

        explosion_increment++;
    }

    if (!finished_tail) {
        if (finished_head) {
            if (pastInterpolations.empty()) {
                finished_tail = true;
            } else if (!pastInterpolations.empty() && increment % 2 == 0) {
                pastInterpolations.erase(pastInterpolations.begin());
            }
        }

        //255, 0, 0 <-
        //195, 105, 0 <-
        //0, 0, 0
        //first transition -> 35 moves
        //second transition -> 15 moves
        sf::Color colorTrail(195, 105, 0);
        sf::Color colorEndTrail(0, 0, 0);
        sf::Color color;
        SpriteSize::Dimensions trailSprite{3, 1.5};
        for (int i = 0; i < pastInterpolations.size(); i++) {

            if (i < pastInterpolations.size() - 15) {
                colorEndTrail.r += 13;
                colorEndTrail.g += 7;
                color = colorEndTrail;
            } else {
                colorTrail.r += 4;
                colorTrail.g -= 7;
                color = colorTrail;
            }

            double trail_rotation{Utilities::convertToDegrees(pastInterpolations[i])};

            Utilities::Coordinates trail_one =
                CollisionDetection::rotateObject(
                    {pastInterpolations[i].x - 9, pastInterpolations[i].y - 2},
                    pastInterpolations[i],
                    trail_rotation
                );

            Utilities::Coordinates trail_two =
                CollisionDetection::rotateObject(
                    {pastInterpolations[i].x - 9, pastInterpolations[i].y + 2},
                    pastInterpolations[i],
                    trail_rotation
                );

            game.appendArray(
                trail_one,
                trailSprite,
                trail_rotation,
                color
            );

            game.appendArray(
                trail_two,
                trailSprite,
                trail_rotation,
                color
            );
        }
    }

    finished_animation = true;
    for (int i = 0; i < explosion.size(); i++) {
        if (!explosion[i]->getFinished()) {
            explosion[i]->update();
            finished_animation = false;
        } else {
            explosion.erase(explosion.begin() + i);
            i--;
        }
    }

    if (finished_animation &&
        finished_head &&
        finished_tail
        ) {
        finished = true;
    }

    render();
    increment++;
}

void BossOne::render() {
    Utilities::SinAndCos sin_and_cos(rotation);

    double largex{SpriteSize::BossOne.x / 3.0}; //large triangle
    double smallx{SpriteSize::BossOne.x / 6.0}; //small triangle
    double bothy{SpriteSize::BossOne.y / 4.0}; //both

    sf::Color color_head(180, 180, 180);
    sf::Color color_tail(120, 120, 120);

    if (hit) {
        hit_increment++;
        if (hit_increment == 6) {hit = false; hit_increment = 0;}
        color_head = sf::Color(250, 250, 250);
        color_tail = sf::Color(190, 190, 190);
    }

    appendTriangleArray(
        currentInterpolation,
        currentInterpolation,
        Utilities::Coordinates{currentInterpolation.x + largex, currentInterpolation.y},
        Utilities::Coordinates{currentInterpolation.x, currentInterpolation.y + bothy},
        sin_and_cos,
        color_head
    );

    appendTriangleArray(
        currentInterpolation,
        currentInterpolation,
        Utilities::Coordinates{currentInterpolation.x + largex, currentInterpolation.y},
        Utilities::Coordinates{currentInterpolation.x, currentInterpolation.y - bothy},
        sin_and_cos,
        color_head
    );

    appendTriangleArray(
        currentInterpolation,
        currentInterpolation,
        Utilities::Coordinates{currentInterpolation.x - largex, currentInterpolation.y},
        Utilities::Coordinates{currentInterpolation.x, currentInterpolation.y + bothy},
        sin_and_cos,
        color_head
    );

    appendTriangleArray(
        currentInterpolation,
        currentInterpolation,
        Utilities::Coordinates{currentInterpolation.x - largex, currentInterpolation.y},
        Utilities::Coordinates{currentInterpolation.x, currentInterpolation.y - bothy},
        sin_and_cos,
        color_head
    );

    Utilities::Coordinates coords = {currentInterpolation.x + largex, currentInterpolation.y};

    appendTriangleArray(
        currentInterpolation,
        Utilities::Coordinates{coords.x, coords.y},
        Utilities::Coordinates{coords.x + smallx, coords.y},
        Utilities::Coordinates{coords.x, coords.y + bothy},
        sin_and_cos,
        color_head
    );

    appendTriangleArray(
        currentInterpolation,
        Utilities::Coordinates{coords.x, coords.y},
        Utilities::Coordinates{coords.x + smallx, coords.y},
        Utilities::Coordinates{coords.x, coords.y - bothy},
        sin_and_cos,
        color_head
    );

    appendTriangleArray(
        currentInterpolation,
        Utilities::Coordinates{coords.x, coords.y},
        Utilities::Coordinates{coords.x, coords.y + bothy},
        Utilities::Coordinates{coords.x - largex, coords.y + bothy},
        sin_and_cos,
        color_head
    );

    appendTriangleArray(
        currentInterpolation,
        Utilities::Coordinates{coords.x, coords.y},
        Utilities::Coordinates{coords.x, coords.y - bothy},
        Utilities::Coordinates{coords.x - largex, coords.y - bothy},
        sin_and_cos,
        color_head
    );

    coords = Utilities::Coordinates{currentInterpolation.x - largex, currentInterpolation.y + bothy};

    appendTriangleArray(
        currentInterpolation,
        Utilities::Coordinates{coords.x, coords.y},
        Utilities::Coordinates{coords.x + largex, coords.y},
        Utilities::Coordinates{coords.x, coords.y + bothy},
        sin_and_cos,
        color_tail
    );

    appendTriangleArray(
        currentInterpolation,
        Utilities::Coordinates{coords.x, coords.y},
        Utilities::Coordinates{coords.x + largex, coords.y},
        Utilities::Coordinates{coords.x, coords.y - bothy},
        sin_and_cos,
        color_tail
    );

    appendTriangleArray(
        currentInterpolation,
        Utilities::Coordinates{coords.x, coords.y},
        Utilities::Coordinates{coords.x - smallx, coords.y},
        Utilities::Coordinates{coords.x, coords.y + bothy},
        sin_and_cos,
        color_tail
    );

    appendTriangleArray(
        currentInterpolation,
        Utilities::Coordinates{coords.x, coords.y},
        Utilities::Coordinates{coords.x - smallx, coords.y},
        Utilities::Coordinates{coords.x, coords.y - bothy},
        sin_and_cos,
        color_tail
    );

    coords = Utilities::Coordinates{currentInterpolation.x - largex, currentInterpolation.y - bothy};

    appendTriangleArray(
        currentInterpolation,
        Utilities::Coordinates{coords.x, coords.y},
        Utilities::Coordinates{coords.x + largex, coords.y},
        Utilities::Coordinates{coords.x, coords.y + bothy},
        sin_and_cos,
        color_tail
    );

    appendTriangleArray(
        currentInterpolation,
        Utilities::Coordinates{coords.x, coords.y},
        Utilities::Coordinates{coords.x + largex, coords.y},
        Utilities::Coordinates{coords.x, coords.y - bothy},
        sin_and_cos,
        color_tail
    );

    appendTriangleArray(
        currentInterpolation,
        Utilities::Coordinates{coords.x, coords.y},
        Utilities::Coordinates{coords.x - smallx, coords.y},
        Utilities::Coordinates{coords.x, coords.y + bothy},
        sin_and_cos,
        color_tail
    );

    appendTriangleArray(
        currentInterpolation,
        Utilities::Coordinates{coords.x, coords.y},
        Utilities::Coordinates{coords.x - smallx, coords.y},
        Utilities::Coordinates{coords.x, coords.y - bothy},
        sin_and_cos,
        color_tail
    );
}

void BossOne::impact() {
    if (!finished_head) {
        finished_head = true;
        MAX_SEGMENT_LENGTH /= 3;
        //insert explosion here
    }
}

void BossOne::fireUpdate() {
    if (!missile_queue.empty() && increment % 6 == 0) {

        Utilities::Coordinates missile_gen =
            CollisionDetection::rotateObject(
                {currentInterpolation.x + 10, currentInterpolation.y},
                currentInterpolation,
                missile_queue.front() + rotation
            );
        Utilities::QuadrantAndSlope q_s = Utilities::convertToQuadrantAndSlope(missile_queue.front() + rotation);
        missile_gen.slope = q_s.slope;
        missile_gen.quadrant = q_s.quadrant;
        missiles.push_back(make_unique<Missile>(
            vertexArray, resolutionScale, game, Utilities::CVBundle{
                missile_gen,
                RO.getSpaceCraft().getCurrentInterpolation(),
                0.0094375,
                2.8
            },
            500
        ));
        
        missile_queue.erase(missile_queue.begin());
    }
}

void BossOne::fireMissile(int num_missiles) {
    if (!finished_head) {
        if (num_missiles % 2 != 0) {num_missiles--;}

        //2 missiles = 90 / 2 = 45
        //4 missiles = 90 / 4 = 22.5 + 45 = 67.5
        //6 missiles = 90 / 6 = 15 + 30 + 30 = 75
        //8 missiles = 90 / 8 = 11.25 + 22.5 + 22.5 + 22.5 = 78.75

        double missile_rotation = 90 / num_missiles;
        bool swap{false};
        for (int i = 0; i < num_missiles; i++) {
            missile_queue.push_back(missile_rotation);
            missile_rotation *= -1;
            if (swap) {
                if (missile_rotation < 0) {
                    missile_rotation -= (180 / num_missiles);
                } else {
                    missile_rotation += (180 / num_missiles);
                }
                swap = false;
            } else {swap = true;}
        }
    }
}

void BossOne::fireMissile() {
    if (!finished_head) {
        missile_queue.push_back(0);
    }
}

void BossOne::reset(Utilities::Coordinates startCoords) {
    missile_queue.clear();
    missiles.clear();
    explosion_rotation = 0;
    hit_increment = 0;
    chaff_increment = MAX_CHAFF_INCREMENT;
    explosion_increment = 0;
    health = MAX_HEALTH;
    hit = false;
    MAX_SEGMENT_LENGTH = 1.25;
    SpaceCraft::reset(startCoords);
}