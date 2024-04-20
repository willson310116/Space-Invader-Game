#pragma once
#include <raylib.h>
#include <vector>
#include "Laser.hpp"

class SpaceShip
{
public:
    SpaceShip();
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
    float fireInterval = 0.5;
    int laserSpeed;

private:
    Texture2D image;
    Vector2 position;
    
    double lastFireTime;
    Sound laserSound;
    
};
