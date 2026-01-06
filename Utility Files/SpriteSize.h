/**
* Lucia Catricks
* Lucia.Catricks@uvm.edu
* CS 2300
* Section A
*/

#ifndef SPRITESIZE_H
#define SPRITESIZE_H

/**
 * Defines the sizes of various sprites. Used for rendering and collision.
 */
namespace SpriteSize {
    /**
     * Define the dimensions of rectangles
     */
    struct Dimensions {
        double x{0};
        double y{0};
    };

    constexpr Dimensions Resolution{640, 360};
    constexpr Dimensions DebrisSprite{3, 1};
    constexpr Dimensions Gunfire{4, 1};
    constexpr Dimensions ExplosionSprite{10, 10};
    constexpr Dimensions ChaffSprite{1, 1};
    constexpr Dimensions Missile{3, 3};
    constexpr Dimensions SpaceCraft{6, 4};
    constexpr Dimensions BossOne{14, 8};
    constexpr Dimensions Healthbar{150, 3};
}

#endif //SPRITESIZE_H
