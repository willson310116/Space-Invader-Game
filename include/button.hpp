#pragma once
#include <raylib.h>

struct Button
{
    Rectangle rect;
    Color color;
    bool IsPressed();
};

inline bool Button::IsPressed()
{
    return CheckCollisionPointRec(GetMousePosition(), rect) && 
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}