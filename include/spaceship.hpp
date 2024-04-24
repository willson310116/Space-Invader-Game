#pragma once
#include <yaml-cpp/yaml.h>
#include <raylib.h>
#include <vector>
#include "Laser.hpp"

class SpaceShip
{
public:
    SpaceShip() = default;
    SpaceShip(YAML::Node& config);
    ~SpaceShip();
    void Draw();
    void Update();
    void MoveLeft();
    void MoveRight();
    void MoveUp();
    void MoveDown();
    void Fire();
    Rectangle GetRect();
    void Reset();

    std::vector<Laser> lasers;
    Vector2 speed;
    float fireInterval;
    int laserSpeed;

private:
    YAML::Node config;
    Texture2D image;
    Vector2 position;
    
    double lastFireTime;
    Sound laserSound;
    
};
