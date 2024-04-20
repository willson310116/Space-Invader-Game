#pragma once
#include <vector>
#include "Button.hpp"

class Menu
{
public:
    virtual void Init() {}
    virtual void Update() {}
    virtual void Draw() {}
    std::vector<Button> buttons;
};