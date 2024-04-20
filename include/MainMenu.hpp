#pragma once
#include <raylib.h>
#include "Menu.hpp"
#include "GameState.hpp"
#include "ButtonHandler.hpp"

extern GameState curGameState;

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
        EXIT,
        NUM_BUTTONS,
    };
};