#pragma once
#include <raylib.h>
#include "button.hpp"
#include "gamestate.hpp"

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
private:
    void InitButton(Button* button, Rectangle rect, Color color);
    bool IsMouseOverButton(Button* button);
    void handleButton(Button* button);
};