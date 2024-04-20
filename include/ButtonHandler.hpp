#pragma once
#include <raylib.h>
#include "Button.hpp"

class ButtonHandler
{
public:
    static void InitButton(Button* button, Rectangle rect, Color color);
    static bool IsMouseOverButton(Button* button);
    static void HandleButton(Button* button);
    static void DrawButton(Button* button, const char* text);
};