#include "MainMenu.hpp"

MainMenu::MainMenu()
{
    Init();
}

void MainMenu::Init()
{
    for (int i = 0; i < NUM_BUTTONS; i++)
        buttons.emplace_back();

    ButtonHandler::InitButton(&buttons[START], (Rectangle){static_cast<float>((GetScreenWidth()-400)/2), 300, 400, 150}, ORANGE);
    ButtonHandler::InitButton(&buttons[EXIT], (Rectangle){static_cast<float>((GetScreenWidth()-400)/2), 500, 400, 150}, ORANGE);
}

void MainMenu::Update()
{   
    for (int i = 0; i < NUM_BUTTONS; i++)
        ButtonHandler::HandleButton(&buttons[i]);

    if (ButtonHandler::IsMouseOverButton(&buttons[START]) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        curGameState = GameState::GAMEPLAY;
    if (ButtonHandler::IsMouseOverButton(&buttons[EXIT]) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        curGameState = GameState::EXIT;
}

void MainMenu::Draw()
{
    DrawText("Space Invader Game !", (GetScreenWidth()-MeasureText("Space Invader Game !", 60))/2, 100, 60, WHITE);
    ButtonHandler::DrawButton(&buttons[START], "Start");
    ButtonHandler::DrawButton(&buttons[EXIT], "Exit");
}
