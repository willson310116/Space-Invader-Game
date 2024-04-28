#pragma once
#include <raylib.h>
#include <yaml-cpp/yaml.h>
#include <unordered_map>
#include "Menu.hpp"
#include "Button.hpp"
#include "GameState.hpp"
#include "ButtonHandler.hpp"
extern std::unordered_map<std::string, Color> colorMap;

class OptionList : public Menu
{
public:
    OptionList() = default;
    OptionList(YAML::Node& config);
    void Init() override;
    void Update() override;
    void Draw() override;
public:
    enum ButtonIndex
    {
        RESTART = 0,
        NEXT,
        MENU,
        EXIT,
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
