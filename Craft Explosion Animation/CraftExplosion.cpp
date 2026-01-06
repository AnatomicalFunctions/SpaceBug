/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#include "CraftExplosion.h"
#include "../Game Logic Package/GameLogic.h"
#include "../CollisionDetection.h"

CraftExplosion::CraftExplosion(
    vector<sf::Vertex>& vertexArray,
    const double resolutionScale,
    GameLogic& game,
    Utilities::Coordinates startCoords,
    double rotation
    ) :
    vertexArray(vertexArray),
    resolutionScale(resolutionScale),
    game(game),
    startCoords(startCoords),
    currentRotation(rotation
    ) {
    sprites.reserve(6); //Growing the vector destroys the internal pointers, so you need to reserve space.
    update();
}

void CraftExplosion::update() {
    Utilities::QuadrantAndSlope q_s_1 = Utilities::convertToQuadrantAndSlope(currentRotation + 20);
    Utilities::QuadrantAndSlope q_s_2 = Utilities::convertToQuadrantAndSlope(currentRotation - 20);
    double primaryRoC = 0.005, secondaryRoC{0}, max_seg_length{0}, mod_1{0}, mod_2{0};
    while (sprites.size() < 6) {
        if (increment == 0) {
            q_s_1 = Utilities::convertToQuadrantAndSlope(currentRotation + 10);
            q_s_2 = Utilities::convertToQuadrantAndSlope(currentRotation - 10);
            //Assume rotation is between -360 and 0
            if (currentRotation + 10 > -90 || currentRotation + 10 < -180) {mod_1 = 100;} else {mod_1 = -100;}
            if (currentRotation - 10 < -90 && currentRotation - 10 > -180) {mod_2 = -100;} else {mod_2 = 100;}
            secondaryRoC = 0.01;
            max_seg_length = 3;
        } else if (increment == 1) {
            q_s_1 = Utilities::convertToQuadrantAndSlope(currentRotation + 20);
            q_s_2 = Utilities::convertToQuadrantAndSlope(currentRotation - 20);
            if (currentRotation + 20 > -90 || currentRotation + 20 < -180) {mod_1 = 100;} else {mod_1 = -100;}
            if (currentRotation - 20 < -90 && currentRotation - 20 > -180) {mod_2 = -100;} else {mod_2 = 100;}
            secondaryRoC = 0.01;
            max_seg_length = 2.75;
        } else if (increment == 2) {
            q_s_1 = Utilities::convertToQuadrantAndSlope(currentRotation + 30);
            q_s_2 = Utilities::convertToQuadrantAndSlope(currentRotation - 30);
            if (currentRotation + 30 > -90 || currentRotation + 30 < -180) {mod_1 = 100;} else {mod_1 = -100;}
            if (currentRotation - 30 < -90 && currentRotation - 30 > -180) {mod_2 = -100;} else {mod_2 = 100;}
            secondaryRoC = 0.01;
            max_seg_length = 2.5;
        }
        sprites.emplace_back(
            vertexArray,
            resolutionScale,
            game,
            Utilities::Coordinates{startCoords.x, startCoords.y, 0, q_s_1.slope, q_s_1.quadrant},
            Utilities::Coordinates{startCoords.x + mod_1, startCoords.y + 1000},
            primaryRoC,
            secondaryRoC,
            max_seg_length);
        sprites.emplace_back(
            vertexArray,
            resolutionScale,
            game,
            Utilities::Coordinates{startCoords.x, startCoords.y, 0, q_s_2.slope, q_s_2.quadrant},
            Utilities::Coordinates{startCoords.x + mod_2, startCoords.y + 1000},
            primaryRoC,
            secondaryRoC,
            max_seg_length);
        increment++;
    }
    finished = true; //tentative assignment
    for (int i = 0; i < sprites.size(); i++) {
        if (!sprites[i].getFinished()) {
            finished = false;
            sprites[i].update();
        }
    }

    // sf::Color color(255, 255, 255, 255);
    // generateCircle(startCoords, color, 1);
    //
    // Utilities::Coordinates c_i = startCoords;
    // double b{CollisionDetection::ObjectPoints::calculateIntercept({c_i.x, c_i.y}, c_i.slope)};
    // for (int i = 0; i < 5; i++) {
    //     c_i.x += 10;
    //     c_i.y = (c_i.x * c_i.slope) + b;
    //     generateCircle({CollisionDetection::rotateObject(
    //         c_i,
    //         startCoords,
    //         Utilities::convertToDegrees(startCoords))},
    //         color,
    //         1
    //     );
    // }

}

void CraftExplosion::generateCircle(
    Utilities::Coordinates ex_coords,
    sf::Color color,
    double size_scale
    ) {
    double rotation{0};
    for (int i = 0; i < 36; i++) {
        Utilities::SinAndCos sin_and_cos(rotation);
        appendTriangleArray(
            ex_coords,
            Utilities::Coordinates{ex_coords.x, ex_coords.y},
            Utilities::Coordinates{
                ex_coords.x + ((SpriteSize::ExplosionSprite.x * size_scale) / 2),
                ex_coords.y + ((SpriteSize::ExplosionSprite.y * size_scale) / 10)
            },
            Utilities::Coordinates{
                ex_coords.x + ((SpriteSize::ExplosionSprite.x * size_scale) / 2),
                ex_coords.y - ((SpriteSize::ExplosionSprite.y * size_scale) / 10)
            },
            sin_and_cos,
            color
        );
        rotation -= 10;
    }
}

void CraftExplosion::appendTriangleArray(
    Utilities::Coordinates zero_point,
    Utilities::Coordinates point_one,
    Utilities::Coordinates point_two,
    Utilities::Coordinates point_three,
    Utilities::SinAndCos sin_and_cos,
    sf::Color color
    ) {
    CollisionDetection::PlainCoordinates c_i_1 =
        {CollisionDetection::preCalculatedRotation(
            point_one, zero_point, sin_and_cos)};
    CollisionDetection::PlainCoordinates c_i_2 =
        {CollisionDetection::preCalculatedRotation(
            point_two, zero_point, sin_and_cos)};
    CollisionDetection::PlainCoordinates c_i_3 =
        {CollisionDetection::preCalculatedRotation(
            point_three, zero_point, sin_and_cos)};

    vertexArray.push_back(sf::Vertex({float(c_i_1.x * resolutionScale), float(c_i_1.y * resolutionScale)}, color));
    vertexArray.push_back(sf::Vertex({float(c_i_2.x * resolutionScale), float(c_i_2.y * resolutionScale)}, color));
    vertexArray.push_back(sf::Vertex({float(c_i_3.x * resolutionScale), float(c_i_3.y * resolutionScale)}, color));
}

bool CraftExplosion::getFinished() const {
    return finished;
}