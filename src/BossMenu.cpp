#include "BossMenu.hpp"
#include <iostream>

BossMenu::BossMenu(YAML::Node& config) : config(config), active(false), transparencyDir(-1)
{
    LoadConfig();
    Init();
}

void BossMenu::LoadConfig()
{
    YAML::Node node = config["Menu"]["BossMenu"];
    buttonWidth = node["ButtonWidth"].as<int>();
    buttonHeight = node["ButtonHeight"].as<int>();
    buttonInitX = (GetScreenWidth() - buttonWidth) / 2;
    buttonInitY = (GetScreenHeight() - node["ButtonYMargin"].as<int>()) / 2;
    if (node["ButtonTexts"].IsSequence())
    {
        for (auto text : node["ButtonTexts"])
            buttonTexts.push_back(text.as<std::string>());
    }
    if (node["ButtonOnColor"].IsSequence())
    {
        for (auto text : node["ButtonOnColor"])
            buttonOnColor.push_back(colorMap[text.as<std::string>()]);
    }
}

void BossMenu::Init()
{
    buttons.emplace_back();
    ButtonHandler::InitButton(&buttons[0],
        (Rectangle){buttonInitX, buttonInitY, buttonWidth, buttonHeight}, RED);
}

void BossMenu::Update()
{
}

void BossMenu::Draw()
{
    ButtonHandler::DrawButton(&buttons[0], buttonTexts[0]);    
    buttons[0].color.a += 5 * transparencyDir;
    if (buttons[0].color.a < 5 || buttons[0].color.a > 250)
        transparencyDir *= -1;
}