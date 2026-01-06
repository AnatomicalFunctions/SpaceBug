/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

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
using namespace std;

/**
 * main program
 * Contains the game loop
 * @return : exit code 0
 */
int main() {
    sf::RenderWindow window;
    vector<sf::Vertex> vertexArray;
    vertexArray.reserve(50000);
    sf::VideoMode mode = Utilities::checkAspectRatio();
    GameLogic game(window, vertexArray, (mode.size.x / 640.0));
    window.create(
        sf::VideoMode(mode.size, 32),
        "SpaceBug",
        sf::Style::None,
        sf::State::Fullscreen
    );

    constexpr double fps{1000.0 / 60.0};

    while (window.isOpen()) {
        auto startLoopTime = std::chrono::high_resolution_clock::now();

        window.clear();
        vertexArray.clear();
        game.checkEvents();
        game.update();
        if (!vertexArray.empty()) {
            window.draw(&vertexArray[0], vertexArray.size(), sf::PrimitiveType::Triangles);
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
