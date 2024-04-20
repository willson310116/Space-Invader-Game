#pragma once
#include <raylib.h>
#include "Button.hpp"
#include "GameState.hpp"
#include "ButtonHandler.hpp"

// TODO: use ploymorphism to replace this and menu class
class OptionList
{
public:
    OptionList();
    void Init();
    void Update();
    void Draw();
    Button restartButton;
    Button exitButton;
    Button menuButton;
};
