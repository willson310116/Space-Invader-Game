#pragma once
#include <raylib.h>
#include "button.hpp"
#include "gamestate.hpp"

extern GameScreen currentScreen;

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
    void InitButton(Button* button, Rectangle rect, Color color);
    bool IsMouseOverButton(Button* button);
    void handleButton(Button* button);
private:
    void DrawButton(Button* button, const char* text);
};
