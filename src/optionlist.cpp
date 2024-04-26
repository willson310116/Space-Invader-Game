#include "OptionList.hpp"

OptionList::OptionList()
{
    Init();
}

void OptionList::Init()
{
    for (int i = 0; i < NUM_BUTTONS; i++)
        buttons.emplace_back();
    ButtonHandler::InitButton(&buttons[RESTART], (Rectangle){static_cast<float>((GetScreenWidth()-300)/2), 150, 300, 100}, ORANGE);
    ButtonHandler::InitButton(&buttons[MENU], (Rectangle){static_cast<float>((GetScreenWidth()-300)/2), 300, 300, 100}, ORANGE);
    ButtonHandler::InitButton(&buttons[EXIT], (Rectangle){static_cast<float>((GetScreenWidth()-300)/2), 450, 300, 100}, ORANGE);
}

void OptionList::Update()
{
    for (int i = 0; i < NUM_BUTTONS; i++)
        ButtonHandler::HandleButton(&buttons[i], ORANGE, BLUE);
}

void OptionList::Draw()
{
    ButtonHandler::DrawButton(&buttons[RESTART], "ReStart");
    ButtonHandler::DrawButton(&buttons[MENU], "Menu");
    ButtonHandler::DrawButton(&buttons[EXIT], "Exit");
}
