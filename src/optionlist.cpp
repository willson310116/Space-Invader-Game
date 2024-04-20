#include "OptionList.hpp"

OptionList::OptionList()
{
    Init();
}

void OptionList::Init()
{
    ButtonHandler::InitButton(&restartButton, (Rectangle){static_cast<float>((GetScreenWidth()-300)/2), 150, 300, 100}, ORANGE);
    ButtonHandler::InitButton(&menuButton, (Rectangle){static_cast<float>((GetScreenWidth()-300)/2), 300, 300, 100}, ORANGE);
    ButtonHandler::InitButton(&exitButton, (Rectangle){static_cast<float>((GetScreenWidth()-300)/2), 450, 300, 100}, ORANGE);
}

void OptionList::Update()
{
    ButtonHandler::HandleButton(&restartButton);
    ButtonHandler::HandleButton(&menuButton);
    ButtonHandler::HandleButton(&exitButton);
}

void OptionList::Draw()
{
    ButtonHandler::DrawButton(&restartButton, "ReStart");
    ButtonHandler::DrawButton(&menuButton, "Menu");
    ButtonHandler::DrawButton(&exitButton, "Exit");
}
