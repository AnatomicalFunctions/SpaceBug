cmake_minimum_required(VERSION 3.30)
project(M2OEP-ncatrick-wnfletch)

set(CMAKE_CXX_STANDARD 20)
set(SFML_DIR "C:/Users/nicka/CLionProjects/SFML-3.0.2-windows-gcc-14.2.0-mingw-32-bit/SFML-3.0.2/lib/cmake/SFML")

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

add_executable(M2OEP-ncatrick-wnfletch main.cpp
        "Coordinate Variables Package/CoordinateInterpretation.cpp"
        "Coordinate Variables Package/CoordinateInterpretation.h"
        "Utility Files/Utilities.h"
        "Utility Files/Utilities.cpp"
        "Coordinate Variables Package/LengthAndSpacing.cpp"
        "Coordinate Variables Package/LengthAndSpacing.h"
        "Coordinate Variables Package/CoordinateVariables.h"
        "Coordinate Variables Package/CoordinateVariables.cpp"
        "Coordinate Variables Package/TranslationHandling.h"
        "Coordinate Variables Package/TranslationHandling.cpp"
        SpaceCraft.cpp
        SpaceCraft.h
        "Game Logic Package/GameLogic.cpp"
        "Game Logic Package/GameLogic.h"
        Missile.cpp
        Missile.h
        "Game Logic Package/EventHandling.cpp"
        "Game Logic Package/EventHandling.h"
        "Game Logic Package/RoundOne.cpp"
        "Game Logic Package/RoundOne.h"
        "Chaff Animation/Chaff.cpp"
        "Chaff Animation/Chaff.h"
        "Chaff Animation/ChaffSprite.cpp"
        "Chaff Animation/ChaffSprite.h"
        "Utility Files/SpriteSize.h"
        "Utility Files/StaticTranslationHandling.cpp"
        "Utility Files/StaticTranslationHandling.h"
        CollisionDetection.cpp
        CollisionDetection.h
        "Craft Explosion Animation/CraftExplosion.cpp"
        "Craft Explosion Animation/CraftExplosion.h"
        "Craft Explosion Animation/DebrisSprite.cpp"
        "Craft Explosion Animation/DebrisSprite.h"
        Gunfire.cpp
        Gunfire.h
        BossOne.cpp
        BossOne.h)
add_executable(M2OEP-ncatrick-wnfletch_testing "Coordinate Variables Package/CoordinateVariablesTesting.cpp"
        "Coordinate Variables Package/CoordinateInterpretation.cpp"
        "Coordinate Variables Package/CoordinateInterpretation.h"
        "Utility Files/Utilities.h"
        "Utility Files/Utilities.cpp"
        "Coordinate Variables Package/LengthAndSpacing.cpp"
        "Coordinate Variables Package/LengthAndSpacing.h"
        "Coordinate Variables Package/CoordinateVariables.h"
        "Coordinate Variables Package/CoordinateVariables.cpp"
        "Coordinate Variables Package/TranslationHandling.h"
        "Coordinate Variables Package/TranslationHandling.cpp"
        SpaceCraft.cpp
        SpaceCraft.h
        "Game Logic Package/GameLogic.cpp"
        "Game Logic Package/GameLogic.h"
        Missile.cpp
        Missile.h
        "Game Logic Package/EventHandling.cpp"
        "Game Logic Package/EventHandling.h"
        "Game Logic Package/RoundOne.cpp"
        "Game Logic Package/RoundOne.h"
        "Chaff Animation/Chaff.cpp"
        "Chaff Animation/Chaff.h"
        "Chaff Animation/ChaffSprite.cpp"
        "Chaff Animation/ChaffSprite.h"
        "Utility Files/SpriteSize.h"
        "Utility Files/StaticTranslationHandling.cpp"
        "Utility Files/StaticTranslationHandling.h"
        CollisionDetection.cpp
        CollisionDetection.h
        "Craft Explosion Animation/CraftExplosion.cpp"
        "Craft Explosion Animation/CraftExplosion.h"
        "Craft Explosion Animation/DebrisSprite.cpp"
        "Craft Explosion Animation/DebrisSprite.h"
        Gunfire.cpp
        Gunfire.h
        BossOne.cpp
        BossOne.h)
add_executable(M2OEP-ncatrick-wnfletch_collisionTesting CollisionTesting.cpp
        "Coordinate Variables Package/CoordinateInterpretation.cpp"
        "Coordinate Variables Package/CoordinateInterpretation.h"
        "Utility Files/Utilities.h"
        "Utility Files/Utilities.cpp"
        "Coordinate Variables Package/LengthAndSpacing.cpp"
        "Coordinate Variables Package/LengthAndSpacing.h"
        "Coordinate Variables Package/CoordinateVariables.h"
        "Coordinate Variables Package/CoordinateVariables.cpp"
        "Coordinate Variables Package/TranslationHandling.h"
        "Coordinate Variables Package/TranslationHandling.cpp"
        SpaceCraft.cpp
        SpaceCraft.h
        "Game Logic Package/GameLogic.cpp"
        "Game Logic Package/GameLogic.h"
        Missile.cpp
        Missile.h
        "Game Logic Package/EventHandling.cpp"
        "Game Logic Package/EventHandling.h"
        "Game Logic Package/RoundOne.cpp"
        "Game Logic Package/RoundOne.h"
        "Chaff Animation/Chaff.cpp"
        "Chaff Animation/Chaff.h"
        "Chaff Animation/ChaffSprite.cpp"
        "Chaff Animation/ChaffSprite.h"
        "Utility Files/SpriteSize.h"
        "Utility Files/StaticTranslationHandling.cpp"
        "Utility Files/StaticTranslationHandling.h"
        CollisionDetection.cpp
        CollisionDetection.h
        "Craft Explosion Animation/CraftExplosion.cpp"
        "Craft Explosion Animation/CraftExplosion.h"
        "Craft Explosion Animation/DebrisSprite.cpp"
        "Craft Explosion Animation/DebrisSprite.h"
        Gunfire.cpp
        Gunfire.h
        BossOne.cpp
        BossOne.h)
target_link_libraries(M2OEP-ncatrick-wnfletch PRIVATE SFML::Graphics SFML::Window SFML::System)
target_link_libraries(M2OEP-ncatrick-wnfletch_testing PRIVATE SFML::Graphics SFML::Window SFML::System)
target_link_libraries(M2OEP-ncatrick-wnfletch_collisionTesting PRIVATE SFML::Graphics SFML::Window SFML::System)