#include "mysteryship.hpp"

Mysteryship::Mysteryship()
{
    image = LoadTexture("../asset/mystery.png");
    alive = false;
}

Mysteryship::~Mysteryship()
{
    UnloadTexture(image);
}

void Mysteryship::Spawn()
{
    position.y = 80;
    int side = GetRandomValue(0, 1);

    if (side == 0)
    {
        position.x = 0;
        speed = 3;
    }
    else
    {
        position.x = GetScreenWidth() - image.width;
        speed = -3;
    }
    alive = true;
}

Rectangle Mysteryship::GetRect()
{
    if (alive)
    {
        return {position.x, position.y,
                static_cast<float>(image.width),
                static_cast<float>(image.height)};
    }
    else
    {
        return {position.x, position.y, 0, 0};
    }
}

void Mysteryship::Update()
{
    if (alive)
    {
        position.x += speed;
        if (position.x > GetScreenWidth() - image.width || position.x < 0)
            alive = false;
    }
}

void Mysteryship::Draw()
{
    if (alive)
        DrawTextureV(image, position, WHITE);
}