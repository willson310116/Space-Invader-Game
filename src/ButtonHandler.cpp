#include "ButtonHandler.hpp"

void ButtonHandler::InitButton(Button *button, Rectangle rect, Color color)
{
    button->rect = rect;
    button->color = color;
}

bool ButtonHandler::IsMouseOverButton(Button *button)
{
    return CheckCollisionPointRec(GetMousePosition(), button->rect);
}

void ButtonHandler::HandleButton(Button *button, Color offColor, Color onColor)
{
    if (IsMouseOverButton(button))
        button->color = onColor;
    else
        button->color = offColor;
}

void ButtonHandler::DrawButton(Button *button, const char *text)
{
    DrawRectangleRec(button->rect, button->color);
    DrawText(text, button->rect.x+button->rect.width/2-MeasureText(text, 40)/2,
            button->rect.y+button->rect.height/2-40/2, 40, WHITE);
}

void ButtonHandler::DrawButton(Button *button, std::string text)
{
    DrawRectangleRec(button->rect, button->color);
    DrawText(text.c_str(), button->rect.x+button->rect.width/2-MeasureText(text.c_str(), 45)/2,
            button->rect.y+button->rect.height/2-40/2, 45, WHITE);
}
