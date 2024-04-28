#pragma once
#include <string>
#include <raylib.h>
#include "Button.hpp"

class ButtonHandler
{
public:
    static void InitButton(Button* button, Rectangle rect, Color color);
    static bool IsMouseOverButton(Button* button);
    static void HandleButton(Button* button, Color onColor, Color offColor);
    static void DrawButton(Button* button, const char* text);
    static void DrawButton(Button *button, std::string text);
};
