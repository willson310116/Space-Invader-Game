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
    void MoveLeft();
    void MoveRight();
    void MoveUp();
    void MoveDown();
    void Fire();

    std::vector<Laser> lasers;

private:
    Texture2D image;
    Vector2 position;
    Vector2 speed;
    double lastFireTime;
};
