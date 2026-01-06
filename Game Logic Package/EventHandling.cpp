/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#include "EventHandling.h"
#include "GameLogic.h"
#include "../SpaceCraft.h"
#include <iostream>

EventHandling::EventHandling(
    sf::RenderWindow& window,
    GameLogic* GL
    ) :
    window(window),
    GL(GL) {
}

void EventHandling::checkEvents() const {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) {
        GL->gunfire();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)) {
        GL->input(SpaceCraft::DEPRESS_RIGHT);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D)) {
        GL->input(SpaceCraft::DEPRESS_LEFT);
    }
    while (const std::optional event = window.pollEvent()) {
        if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {
            checkMovementReleased(keyReleased);
        } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            checkOther(keyPressed);
        }
    }
}

void EventHandling::checkMovementReleased(const sf::Event::KeyReleased* keyReleased) const {
    if (keyReleased->scancode == sf::Keyboard::Scancode::A) {
        GL->input(SpaceCraft::RELEASE_RIGHT);
    } else if (keyReleased->scancode == sf::Keyboard::Scancode::D) {
        GL->input(SpaceCraft::RELEASE_LEFT);
    } else if (keyReleased->scancode == sf::Keyboard::Scancode::W) {
        GL->resetGunfireIncrement();
    }
}

//sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D)

void EventHandling::checkOther(const sf::Event::KeyPressed* keyPressed) const {
    if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
        window.close();
    } else if (keyPressed->scancode == sf::Keyboard::Scancode::Space) {
        GL->reset();
    } else if (keyPressed->scancode == sf::Keyboard::Scancode::LShift) {
        GL->chaff();
    }
    // else if (keyPressed->scancode == sf::Keyboard::Scancode::LShift) {
    //     GL->explode();
    // }
}
