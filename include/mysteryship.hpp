#pragma once
#include <raylib.h>
#include <yaml-cpp/yaml.h>

class MysteryShip
{
public:
    MysteryShip() = default;
    MysteryShip(YAML::Node& config);
    ~MysteryShip();
    void Update();
    void Draw();
    void Spawn();
    Rectangle GetRect();
    bool alive;
    
private:
    Vector2 position;
    Texture2D image;
    int speed;
};