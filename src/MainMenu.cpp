#include <string>
#include "MainMenu.hpp"

MainMenu::MainMenu() : level(1)
{
    Init();
}

void MainMenu::Init()
{
    for (int i = 0; i < NUM_BUTTONS; i++)
        buttons.emplace_back();

    ButtonHandler::InitButton(&buttons[START], (Rectangle){static_cast<float>((GetScreenWidth()-400)/2), 300, 400, 100}, ORANGE);
    ButtonHandler::InitButton(&buttons[LEVEL], (Rectangle){static_cast<float>((GetScreenWidth()-400)/2)+100, 450, 200, 100}, ORANGE);
    ButtonHandler::InitButton(&buttons[MINUS_LEVEL], (Rectangle){static_cast<float>((GetScreenWidth()-400)/2), 450, 100, 100}, RED);
    ButtonHandler::InitButton(&buttons[PLUS_LEVEL], (Rectangle){static_cast<float>((GetScreenWidth()-400)/2)+300, 450, 100, 100}, GREEN);
    ButtonHandler::InitButton(&buttons[EXIT], (Rectangle){static_cast<float>((GetScreenWidth()-400)/2), 600, 400, 100}, ORANGE);
}

void MainMenu::Update()
{   
    ButtonHandler::HandleButton(&buttons[START], ORANGE, BLUE);
    ButtonHandler::HandleButton(&buttons[PLUS_LEVEL], ORANGE, GREEN);
    ButtonHandler::HandleButton(&buttons[MINUS_LEVEL], ORANGE, RED);
    ButtonHandler::HandleButton(&buttons[EXIT], ORANGE, BLUE);

    if (buttons[START].IsPressed())
    {
        curGameState = GameState::GAMEPLAY;
        loadFlag = true;
    }
        
    if (buttons[EXIT].IsPressed())
        curGameState = GameState::EXIT;
    if (buttons[PLUS_LEVEL].IsPressed())
        if (level < 3)
            level += 1;
    if (buttons[MINUS_LEVEL].IsPressed())
        if (level > 1)
            level -= 1;
}

void MainMenu::Draw()
{
    DrawText("Space Invader Game !", (GetScreenWidth()-MeasureText("Space Invader Game !", 60))/2, 100, 60, WHITE);
    ButtonHandler::DrawButton(&buttons[START], "Start");
    ButtonHandler::DrawButton(&buttons[LEVEL], std::to_string(level).c_str());
    ButtonHandler::DrawButton(&buttons[PLUS_LEVEL], "+");
    ButtonHandler::DrawButton(&buttons[MINUS_LEVEL], "-");
    ButtonHandler::DrawButton(&buttons[EXIT], "Exit");
}
