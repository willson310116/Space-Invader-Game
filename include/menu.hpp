#pragma once
#include <raylib.h>
#include "Button.hpp"
#include "GameState.hpp"
#include "ButtonHandler.hpp"

extern GameState curGameState;

class Menu
{
public:
    Menu();
    void Init();
    void Update();
    void Draw();
    Button startButton;
    Button exitButton;
};