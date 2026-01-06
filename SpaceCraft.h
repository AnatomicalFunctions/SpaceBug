/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#ifndef SPACECRAFT_H
#define SPACECRAFT_H

#include "Coordinate Variables Package/CoordinateVariables.h"
#include <SFML/Graphics.hpp>

#include "Gunfire.h"
#include "Chaff Animation/Chaff.h"
class GameLogic;
class CraftExplosion;

/**
 * Class to extend CoordinateVariables to contain code necessary to define the movement of the player.
 * Includes functions that overwrite VCoords in "real time" depending on an inputting index.
 */
class SpaceCraft : public CoordinateVariables {
public:
    enum InputType {DEPRESS_LEFT, RELEASE_LEFT, DEPRESS_RIGHT, RELEASE_RIGHT};
    //Defines the distance traveled by the spacecraft before updating the rate of change ("a").
    //Must use an int to define
    const double MAX_ARC_LENGTH{MAX_SEGMENT_LENGTH * 2};
    //defines an increasing ROC depending on how long the SpaceCraft has been turning
    //"how long" defined by MAX_ARC_LENGTH
    const double ROC_ONE;
    const double ROC_TWO;
protected:
    vector<unique_ptr<Chaff>> chaff;
    vector<unique_ptr<CraftExplosion>> explosion;
    vector<unique_ptr<Gunfire>> gunfire;
    bool chaffExist{false};
    bool gunfireExist{false};
    bool gunfire_direction_switch{false};
    int gunfire_increment{0}; //Used to delay fire between shots and reset to allow instant fire upon new input.
    int gunfire_spread{0};
    vector<sf::Vertex>& vertexArray;
    const double resolutionScale;
    GameLogic& game;
    Utilities::Coordinates currentInterpolation;
    vector<Utilities::Coordinates> pastInterpolations;
    int increment{0};
    double rotation{0};
    bool left_depressed{false}; //Current keyboard inputs
    bool right_depressed{false};
    bool finished_head{false};
    bool finished_tail{false};
    bool finished_animation{false};
    bool finished{false};
    static constexpr int MAX_TRAIL_LENGTH{25};
public:

    /**
     * Passes information to a matching constructor in CoordinateVariables
     * Initializes spaceCraftShape
     * And then calls SpaceCraft::initialization()
     */
    SpaceCraft(
        vector<sf::Vertex>& vertexArray,
        double resolutionScale,
        GameLogic& game,
        Utilities::Coordinates startCoords,
        double ROC_ONE,
        double ROC_TWO,
        double MAX_SEGMENT_LENGTH
    );

    /**
     * Updates object information
     * Cycles through chaff vector and sees if there are any active Chaff objects
     * Calls chaffErase on any Chaff objects that have marked themselves finished.
     * Requires: currentInterpolation, spaceCraftShape, resolutionScale, chaff, chaffErase(),
     *           Utilities::convertToDegrees()
     * Modifies: currentInterpolation
     */
    virtual void update();

    virtual void render();

    /**
     * For when you want the SpaceCraft to blow up.
     */
    virtual void impact();

    /**
     * For when the SpaceCraft has hit the border
     */
    void impactBorderUpDown();

    /**
     * For when the SpaceCraft has hit the border
     */
    void impactBorderLeftRight();

    /**
     * Instant respawn!
     */
    virtual void reset(
        Utilities::Coordinates startCoords
    );

    void appendTriangleArray(
        const Utilities::Coordinates &zero_point,
        const Utilities::Coordinates &point_one,
        const Utilities::Coordinates &point_two,
        const Utilities::Coordinates &point_three,
        const Utilities::SinAndCos &sin_and_cos,
        sf::Color color
    ) const;

    /**
     * Processes new input to overwrite VCoords and generate a series of arcs.
     * Validates and records input.
     * Requires: depression(), release(), turnIn(), turnOut(), startCoords, vCoords, setVCoordsClearToIndex(),
     *           interpolationIncrement, left_depressed, right_depressed
     * Modifies: vCoords, startCoords, quadrant, slope, left_depressed, right_depressed
     * @param inputType : enum defining the nature of the input
     */
    void input(InputType inputType);

    /**
     * Defines the behavior given a LEFT turn or a RIGHT turn.
     * Accounts for current RoC
     * Requires: ROC_ONE, ROC_TWO, MAX_ARC_LENGTH, rocArcGenRecursion(), vCoords, turnIn(), turnOut()
     * Modifies: a
     * @param index : index matching a Coordinates structure in VCoords
     * @param direction_modifier : enum defining direction of turn
     */
    void depression(int index, int direction_modifier);

    /**
     * Defines the behavior given release from a LEFT or a RIGHT turn
     * Requires: ROC_ONE, MAX_ARC_LENGTH, rocArcGenRecursion(), vCoords, turnIn()
     * Modifies: a
     * @param index : index matching a Coordinates structure in vCoords
     * @param direction_modifier : enum defining direction of turn
     */
    void release(int index, int direction_modifier);

    /**
     * Function that defines moving from turning back to straight
     * Requires: ROC_ONE, MAX_ARC_LENGTH, rocArcGenRecursion()
     * Modifies: a
     * @param direction_modifier : enum defining direction of turn.
     */
    void turnIn(int direction_modifier);

    /**
     * Function that defines moving from straight to turning
     * Requires: ROC_ONE, ROC_TWO, MAX_ARC_LENGTH, rocArcGenRecursion()
     * Modifies: a
     * @param direction_modifier : enum defining direction of turn
     */
    void turnOut(int direction_modifier);

    /**
     * Sets "a" to zero and calls rocArcGenRecursion(MAX_ARC_LENGTH) once to populate vCoords
     */
    void initialization() override;

    /**
     * Specifies arcGen behavior when called on a SpaceCraft object
     * Requires: rocArcGenRecursion(), MAX_ARC_LENGTH
     */
    void arcGenCall() override;

    /**
     * Adds a unique_ptr to a Chaff object to the chaff vector when called.
     * Also only generates a chaff if there are none on the board
     * Modifies: chaff
     */
    void chaffGen();

    void gunfireGen();

    void resetGunfireIncrement();

    /**
     * Called erase function on a specific index in chaff vector.
     * @param index : index in chaff
     */
    void chaffErase(int index);

    //Getters
    Utilities::Coordinates getCurrentInterpolation() const;
    double getRotation() const;
    bool getFinished() const;
    Utilities::Coordinates getChaffCenter() const;
    bool getChaffExist() const;

    const vector<unique_ptr<Gunfire>>& getGunfire() const;

    //Setters
    void setCurrentInterpolation(Utilities::Coordinates currentInterpolation);
};



#endif //SPACECRAFT_H
