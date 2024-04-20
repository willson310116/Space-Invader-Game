#include "OptionList.hpp"

OptionList::OptionList()
{
    Init();
}

void OptionList::Init()
{
    for (int i = 0; i < NUM_BUTTONS; i++)
        buttons.push_back(Button());
    ButtonHandler::InitButton(&buttons[START], (Rectangle){static_cast<float>((GetScreenWidth()-300)/2), 150, 300, 100}, ORANGE);
    ButtonHandler::InitButton(&buttons[MENU], (Rectangle){static_cast<float>((GetScreenWidth()-300)/2), 300, 300, 100}, ORANGE);
    ButtonHandler::InitButton(&buttons[EXIT], (Rectangle){static_cast<float>((GetScreenWidth()-300)/2), 450, 300, 100}, ORANGE);
}

void OptionList::Update()
{
    for (int i = 0; i < NUM_BUTTONS; i++)
        ButtonHandler::HandleButton(&buttons[i]);
}

void OptionList::Draw()
{
    ButtonHandler::DrawButton(&buttons[START], "ReStart");
    ButtonHandler::DrawButton(&buttons[MENU], "Menu");
    ButtonHandler::DrawButton(&buttons[EXIT], "Exit");
}
