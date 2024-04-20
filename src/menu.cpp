#include "Menu.hpp"

Menu::Menu()
{
    Init();
}

void Menu::Init()
{
    ButtonHandler::InitButton(&startButton, (Rectangle){static_cast<float>((GetScreenWidth()-400)/2), 300, 400, 150}, ORANGE);
    ButtonHandler::InitButton(&exitButton, (Rectangle){static_cast<float>((GetScreenWidth()-400)/2), 500, 400, 150}, ORANGE);
}

void Menu::Update()
{   
    ButtonHandler::HandleButton(&startButton);
    ButtonHandler::HandleButton(&exitButton);

    if (ButtonHandler::IsMouseOverButton(&startButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        curGameState = GameState::GAMEPLAY;
    if (ButtonHandler::IsMouseOverButton(&exitButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        curGameState = GameState::EXIT;
}

void Menu::Draw()
{
    DrawText("Space Invader Game !", (GetScreenWidth()-MeasureText("Space Invader Game !", 60))/2, 100, 60, WHITE);
    ButtonHandler::DrawButton(&startButton, "Start");
    ButtonHandler::DrawButton(&exitButton, "Exit");
}
