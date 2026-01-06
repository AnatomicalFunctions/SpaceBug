/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#include "CollisionDetection.h"
#include "Coordinate Variables Package/CoordinateVariables.h"
#include "Coordinate Variables Package/CoordinateInterpretation.h"
#include "Coordinate Variables Package/LengthAndSpacing.h"
#include "SpaceCraft.h"
#include "Game Logic Package/GameLogic.h"
#include "Missile.h"
#include <fstream>
#include <iostream>
#include <format>
#include <sstream>
#include <string>
#include "Utility Files/Utilities.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <thread>
#include <chrono>
#include "Utility Files/StaticTranslationHandling.h"
using namespace std;

/**
 * main program
 * Contains the game loop
 * @return : exit code 0
 */
int main() {
    sf::RenderWindow window;
    sf::VertexArray vertexArray;
    vertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);
    sf::VideoMode mode = Utilities::checkAspectRatio();
    GameLogic game(window, vertexArray, (mode.size.x / 640.0));
    window.create(
        sf::VideoMode(mode.size, 32),
        "SpaceBug",
        sf::Style::None,
        sf::State::Fullscreen
    );

    constexpr double fps{1000.0 / 60.0};

    sf::RectangleShape collisionShape;
    sf::RectangleShape collisionShape2;
    sf::RectangleShape collisionShape3;
    collisionShape.setSize({float(12 * game.getResolutionScale()), float(12 * game.getResolutionScale())});
    collisionShape2.setSize({float(6 * game.getResolutionScale()), float(6 * game.getResolutionScale())});
    collisionShape3.setSize({float(SpriteSize::SpaceCraft.x * game.getResolutionScale()), float(SpriteSize::SpaceCraft.y * game.getResolutionScale())});

    collisionShape.setFillColor(sf::Color::White);
    collisionShape2.setFillColor(sf::Color::White);
    collisionShape3.setFillColor(sf::Color::Red);
    collisionShape.setPosition({float(160 * game.getResolutionScale()),
                                     float(90 * game.getResolutionScale())});
    collisionShape2.setPosition({float(280 * game.getResolutionScale()),
                                     float(90 * game.getResolutionScale())});

    //Utilities::Coordinates c_i = game.RO.getSpaceCraft().getCurrentInterpolation();
    // if (CollisionDetection::checkCollision({220, 90}, Dimensions{10, 3}, 0.5, {160, 90}, Dimensions{12, 12}, 0)) {
    //     cout << "COLLISION" << endl;
    // } else {
    //     cout << "MISS" << endl;
    // }

    while (window.isOpen()) {
        auto startLoopTime = std::chrono::high_resolution_clock::now();

        window.clear();
        vertexArray.clear();
        game.checkEvents();
        game.update();

        Utilities::Coordinates c_i = game.RO.getSpaceCraft().getCurrentInterpolation();

        window.draw(collisionShape);
        window.draw(vertexArray);
        window.display();

        if (CollisionDetection::checkCollision(c_i, SpriteSize::SpaceCraft, game.RO.getSpaceCraft().getRotation(), Utilities::Coordinates{160, 90}, SpriteSize::Dimensions{12, 12}, 0)) {
             window.draw(collisionShape2);
        }

        window.display();

        auto endLoopTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> frameTime = endLoopTime - startLoopTime;
        if (frameTime.count() < fps) {
            this_thread::sleep_for(std::chrono::duration<double, std::milli> (fps - frameTime.count()));
        }
    }
    return 0;
}