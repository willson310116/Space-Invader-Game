#include "optionlist.hpp"

void OptionList::InitButton(Button* button, Rectangle rect, Color color)
{
    button->rect = rect;
    button->color = color;
}

bool OptionList::IsMouseOverButton(Button* button)
{
    return CheckCollisionPointRec(GetMousePosition(), button->rect);
}

void OptionList::handleButton(Button* button)
{
    if (IsMouseOverButton(button))
        button->color = BLUE;
    else
        button->color = ORANGE;
}

OptionList::OptionList()
{
    Init();
}

void OptionList::Init()
{
    InitButton(&restartButton, (Rectangle){float((GetScreenWidth()-300)/2), 150, 300, 100}, ORANGE);
    InitButton(&menuButton, (Rectangle){float((GetScreenWidth()-300)/2), 300, 300, 100}, ORANGE);
    InitButton(&exitButton, (Rectangle){float((GetScreenWidth()-300)/2), 450, 300, 100}, ORANGE);
}

void OptionList::Update()
{
    handleButton(&restartButton);
    handleButton(&menuButton);
    handleButton(&exitButton);
}

void OptionList::Draw()
{
    DrawButton(&restartButton, "ReStart");
    DrawButton(&menuButton, "Menu");
    DrawButton(&exitButton, "Exit");
}

void OptionList::DrawButton(Button* button, const char* text)
{
    DrawRectangleRec(button->rect, button->color);
    DrawText(text, button->rect.x+button->rect.width/2-MeasureText(text, 40)/2,
            button->rect.y+button->rect.height/2-40/2, 40, WHITE);
}
