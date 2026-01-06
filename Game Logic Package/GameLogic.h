/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <SFML/Graphics/RenderWindow.hpp>
#include "EventHandling.h"
#include "../SpaceCraft.h"
#include "RoundOne.h"
#include "../Utility Files/Utilities.h"

/**
 *
 */
class GameLogic {
public:
    enum Round {ROUND_ONE};
private:
    vector<sf::Vertex>& vertexArray;
    const double resolutionScale; //Internal resolution is 640x360
    EventHandling EH;
    Round currentRound{ROUND_ONE};
public:
    RoundOne RO;//unprivated for testing

    GameLogic(sf::RenderWindow& window, vector<sf::Vertex>& vertexArray, double resolutionScale);

    void checkEvents() const;

    void update();

    /**
     *
     * Requires:
     * Modifies:
     * @param inputType :
     */
    void input(SpaceCraft::InputType inputType);

    void chaff();

    void gunfire();

    void resetGunfireIncrement();

    void explode();

    void appendArray(
        const Utilities::Coordinates &interpolation,
        const SpriteSize::Dimensions &dimensions,
        double rotation,
        const sf::Color &color
    ) const;

    void reset();

    //Getters
    double getResolutionScale() const;
    Round getCurrentRound() const;

    //Setters
    void setCurrentRound(Round currentRound);
};



#endif //GAMELOGIC_H
