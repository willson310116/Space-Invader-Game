#include <iostream>
#include "Laser.hpp"

Laser::Laser(Vector2 position, int speed, int type) :
    position(position), speed(speed), active(true)
{
    // spaceship
    switch (type)
    {
    case 1:
        color = YELLOW;
        break;
    case 2:
        color = RED;
        break;
    case 3:
        color = PURPLE;
        break;
    default:
        color = YELLOW;
    }
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
        if (position.y > GetScreenHeight() - 100 || position.y < 25)
        {
            active = false;
        }
    }
}