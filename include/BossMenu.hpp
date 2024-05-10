#pragma once
#include <raylib.h>
#include <yaml-cpp/yaml.h>
#include <unordered_map>
#include "Menu.hpp"
#include "Button.hpp"
#include "ButtonHandler.hpp"

extern std::unordered_map<std::string, Color> colorMap;

class BossMenu : public Menu
{
public:
    // add flag for animation
    // 
    BossMenu() = default;
    BossMenu(YAML::Node& config);
    void Init() override;
    void Update() override;
    void Draw() override;
    void LoadConfig();
    bool active;
public:
    enum ButtonIndex
    {
        WARNING = 0,
        NUM_BUTTONS,
    };
private:
    YAML::Node config;
    float buttonInitX;
    float buttonInitY;
    float buttonWidth;
    float buttonHeight;
    std::vector<std::string> buttonTexts;
    std::vector<Color> buttonOnColor;
    int transparencyDir;
};

