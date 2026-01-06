/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#ifndef EVENTHANDLING_H
#define EVENTHANDLING_H

#include <SFML/Graphics.hpp>

class GameLogic;

/**
 * Contains the code for polling for new input in each cycle.
 */
class EventHandling {
private:
    sf::RenderWindow& window;
    GameLogic* GL;
public:
    /**
     * Default constructor
     */
    EventHandling(sf::RenderWindow& window, GameLogic* GL);

    /**
     * Checks for keyPressed and keyReleased events and then passes call onto other functions.
     */
    void checkEvents() const;

    /**
     * Passes released key updates to the current SpaceCraft through GameLogic
     */
    void checkMovementReleased(const sf::Event::KeyReleased* keyReleased) const;

    /**
     * Checks other key inputs... very barebones right now, used for testing.
     */
    void checkOther(const sf::Event::KeyPressed* keyPressed) const;
};



#endif //EVENTHANDLING_H
