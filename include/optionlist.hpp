#pragma once
#include <raylib.h>
#include "Menu.hpp"
#include "Button.hpp"
#include "GameState.hpp"
#include "ButtonHandler.hpp"

class OptionList : public Menu
{
public:
    OptionList();
    void Init() override;
    void Update() override;
    void Draw() override;
    enum ButtonIndex
    {
        START = 0,
        MENU,
        EXIT,
        NUM_BUTTONS,
    };
};
