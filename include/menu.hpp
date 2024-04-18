#pragma once
#include <raylib.h>

struct Button
{
    Rectangle rect;
    Color color;
    bool clicked;
}; 



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