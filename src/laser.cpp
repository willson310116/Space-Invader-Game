#include <iostream>
#include "laser.hpp"

Laser::Laser(Vector2 position, int speed, int type) :
    position(position), speed(speed), active(true)
{
    // spaceship
    if (type == 1)
        color = {243, 216, 63, 255};
    else
        color = {240, 68, 51, 255};
}

Laser::~Laser()
{
}

void Laser::Draw()
{
    if (active)
        DrawRectangle(position.x, position.y, 4, 15, color);
}

Rectangle Laser::GetRect()
{
    return {position.x, position.y, 4, 15};
}

void Laser::Update()
{
    position.y += speed;
    if (active)
    {
        if (position.y > GetScreenHeight() || position.y < 0)
        {
            active = false;
            std::cout << "Laser inactive!" << std::endl;
        }
    }
}