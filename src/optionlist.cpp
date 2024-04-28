#include "OptionList.hpp"
#include <iostream>

OptionList::OptionList(YAML::Node& config) : config(config)
{
    LoadConfig();
    Init();
}

void OptionList::LoadConfig()
{
    YAML::Node node = config["Menu"]["OptionList"];
    buttonInitX = (GetScreenWidth() - node["ButtonXMargin"].as<int>()) / 2;
    buttonInitY = (GetScreenHeight() - node["ButtonYMargin"].as<int>()) / 2;
    buttonWidth = node["ButtonWidth"].as<int>();
    buttonHeight = node["ButtonHeight"].as<int>();
    buttonYGap = node["ButtonYGap"].as<int>();
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
    if (node["ButtonOffColor"].IsSequence())
    {
        for (auto text : node["ButtonOffColor"])
            buttonOffColor.push_back(colorMap[text.as<std::string>()]);
    } 
    
}

void OptionList::Init()
{
    for (int i = 0; i < NUM_BUTTONS; i++)
    {
        buttons.emplace_back();
        ButtonHandler::InitButton(&buttons[i],
            (Rectangle){buttonInitX, buttonInitY + i * (buttonHeight + buttonYGap),
                buttonWidth, buttonHeight}, buttonOffColor[i]);
    }    
}

void OptionList::Update()
{
    for (int i = 0; i < NUM_BUTTONS; i++)
        ButtonHandler::HandleButton(&buttons[i], buttonOffColor[i], buttonOnColor[i]);
}

void OptionList::Draw()
{
    for (int i = 0; i < NUM_BUTTONS; i++)
        ButtonHandler::DrawButton(&buttons[i], buttonTexts[i]);
}
