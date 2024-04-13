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