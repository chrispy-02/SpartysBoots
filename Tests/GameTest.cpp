/**
* @file GameTest.cpp
*
* @author Bob Wilson
*/


#include <pch.h>

#include <Game.h>
#include "gtest/gtest.h"
#include <iostream>
#include <fstream>
#include <wx/filename.h>
#include <string>


using namespace std;


TEST(GameTest, Construct)
{
    Game game;
}


TEST(GameTest, Load)
{
    Game game;
    ASSERT_EQ(game.NumberOfItems(), 0) << "Checking there are no items before loading a level";

    game.Load(L"levels/level1.xml");
    ASSERT_EQ(game.NumberOfItems(), 11) << "Check level 1 has 11 items";
    ASSERT_EQ(game.GetConveyorSpeed(), 100) << "Checking conveyor speed 100";
    ASSERT_EQ(game.GetWidth(), 1150) << "Checking width of game is 1150";
    ASSERT_EQ(game.GetHeight(), 800) << "Checking height of game is 800";
    ASSERT_EQ(game.GetBeamY(), 437) << "Checking beam Y value is 437";
    ASSERT_EQ(game.GetKickSpeed(), 1000) << "Checking kick speed is 1000";

    game.Load(L"levels/level2.xml");
    ASSERT_EQ(game.NumberOfItems(), 14) << "Check level 2 has 14 items";

    game.Load(L"levels/level3.xml");
    ASSERT_EQ(game.NumberOfItems(), 14) << "Check level 2 has 14 items";

    game.Load(L"levels/level4.xml");
    ASSERT_EQ(game.NumberOfItems(), 14) << "Check level 4 has 14 items";

    game.Load(L"levels/level5.xml");
    ASSERT_EQ(game.NumberOfItems(), 14) << "Check level 5 has 14 items";

    game.Load(L"levels/level6.xml");
    ASSERT_EQ(game.NumberOfItems(), 14) << "Check level 6 has 14 items";
}
