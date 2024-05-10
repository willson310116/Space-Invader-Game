#include <iostream>
#include "MainMenu.hpp"

MainMenu::MainMenu(YAML::Node &config) : level(1), config(config)
{
    LoadConfig();
    Init();
}

void MainMenu::LoadConfig()
{
    YAML::Node node = config["Menu"]["MainMenu"];
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

void MainMenu::Init()
{
    for (int i = 0; i < NUM_BUTTONS; i++)
        buttons.emplace_back();
    
    for (int i = 0; i < NUM_BUTTONS-2; i++)
        ButtonHandler::InitButton(&buttons[i],
                (Rectangle){buttonInitX, buttonInitY + i * (buttonHeight + buttonYGap),
                    buttonWidth, buttonHeight}, buttonOffColor[i]);
        
    ButtonHandler::InitButton(&buttons[MINUS_LEVEL], (Rectangle){buttonInitX, 400, 100, 100}, buttonOffColor[MINUS_LEVEL]);
    ButtonHandler::InitButton(&buttons[PLUS_LEVEL], (Rectangle){buttonInitX+300, 400, 100, 100}, buttonOffColor[PLUS_LEVEL]);
}

void MainMenu::Update()
{   
    for (int i = 0; i < NUM_BUTTONS; i++)
    {
        if (i == LEVEL) continue;
        ButtonHandler::HandleButton(&buttons[i], buttonOffColor[i], buttonOnColor[i]);
    }
        
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
    std::string levelText = "Level " + std::to_string(level);
    for (int i = 0; i < NUM_BUTTONS; i++)
    {
        if (i == LEVEL)
            ButtonHandler::DrawButton(&buttons[i], levelText);
        else
            ButtonHandler::DrawButton(&buttons[i], buttonTexts[i]);
    }
}

