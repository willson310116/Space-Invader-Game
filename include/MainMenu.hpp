#pragma once
#include <raylib.h>
#include <yaml-cpp/yaml.h>
#include <unordered_map>
#include "Menu.hpp"
#include "GameState.hpp"
#include "ButtonHandler.hpp"

extern GameState curGameState;
extern bool loadFlag;
extern std::unordered_map<std::string, Color> colorMap;

class MainMenu : public Menu
{
public:
    MainMenu() = default;
    MainMenu(YAML::Node& config);
    void Init() override;
    void Update() override;
    void Draw() override;
public:
    int level;
    enum ButtonIndex
    {
        START = 0,
        LEVEL,
        EXIT,
        PLUS_LEVEL,
        MINUS_LEVEL,
        NUM_BUTTONS,
    };
    
private:
    YAML::Node config;
    float buttonInitX;
    float buttonInitY;
    float buttonWidth;
    float buttonHeight;
    float buttonYGap;
    std::vector<std::string> buttonTexts;
    std::vector<Color> buttonOnColor;
    std::vector<Color> buttonOffColor;
private:
    void LoadConfig();
};