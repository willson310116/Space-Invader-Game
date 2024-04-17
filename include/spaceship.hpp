#pragma once
#include <raylib.h>
#include <vector>
#include "laser.hpp"

class Spaceship
{
public:
    Spaceship();
    ~Spaceship();
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

private:
    Texture2D image;
    Vector2 position;
    Vector2 speed;
    double lastFireTime;
    Sound laserSound;
};
