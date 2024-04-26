#pragma once
#include <raylib.h>
#include "Menu.hpp"
#include "GameState.hpp"
#include "ButtonHandler.hpp"

extern GameState curGameState;
extern bool loadFlag;

class MainMenu : public Menu
{
public:
    MainMenu();
    void Init() override;
    void Update() override;
    void Draw() override;
    enum ButtonIndex
    {
        START = 0,
        LEVEL,
        PLUS_LEVEL,
        MINUS_LEVEL,
        EXIT,
        NUM_BUTTONS,
    };
public:
    int level;
    // bool loadFlag;
};