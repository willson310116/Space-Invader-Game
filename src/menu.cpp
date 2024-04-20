#include "menu.hpp"

void Menu::InitButton(Button* button, Rectangle rect, Color color)
{
    button->rect = rect;
    button->color = color;
}

bool Menu::IsMouseOverButton(Button* button)
{
    return CheckCollisionPointRec(GetMousePosition(), button->rect);
}

void Menu::handleButton(Button* button)
{
    if (IsMouseOverButton(button))
        button->color = BLUE;
    else
        button->color = ORANGE;
}

Menu::Menu()
{
    Init();
}

void Menu::Init()
{
    InitButton(&startButton, (Rectangle){float((GetScreenWidth()-400)/2), 300, 400, 150}, ORANGE);
    InitButton(&exitButton, (Rectangle){float((GetScreenWidth()-400)/2), 500, 400, 150}, ORANGE);
}

void Menu::Update()
{
    handleButton(&startButton);
    handleButton(&exitButton);
    if (IsMouseOverButton(&startButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        curGameState = GameState::GAMEPLAY;
    if (IsMouseOverButton(&exitButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        curGameState = GameState::EXIT;
}

void Menu::Draw()
{
    DrawRectangleRec(startButton.rect, startButton.color);
    DrawRectangleRec(exitButton.rect, exitButton.color);

    DrawText("Space Invader Game !", (GetScreenWidth()-MeasureText("Space Invader Game !", 60))/2, 100, 60, WHITE);

    DrawText("Start", startButton.rect.x+startButton.rect.width/2-MeasureText("Start", 40)/2,
            startButton.rect.y+startButton.rect.height/2-40/2, 40, WHITE);
    DrawText("Exit", exitButton.rect.x+exitButton.rect.width/2-MeasureText("Exit", 40)/2,
            exitButton.rect.y+exitButton.rect.height/2-40/2, 40, WHITE);
}
