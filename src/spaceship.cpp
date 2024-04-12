#include "spaceship.hpp"

Spaceship::Spaceship()
{
    image = LoadTexture("../asset/spaceship.png");
    position.x = (GetScreenWidth() - image.width) / 2;
    position.y = GetScreenHeight() - image.height;
    speed.x = 7;
    speed.y = 7;
}

Spaceship::~Spaceship()
{
    UnloadTexture(image);
}

void Spaceship::Draw()
{
    DrawTextureV(image, position, WHITE);
}

void Spaceship::MoveLeft()
{
    position.x -= speed.x;
    if (position.x < 0)
        position.x = 0;
}

void Spaceship::MoveRight()
{
    position.x += speed.x;
    if (position.x > GetScreenWidth() - image.width)
        position.x = GetScreenWidth() - image.width;
}

void Spaceship::MoveUp()
{
    position.y -= speed.y;
    if (position.y < 0)
        position.y = 0;
}

void Spaceship::MoveDown()
{
    position.y += speed.y;
    if (position.y > GetScreenHeight() - image.height)
        position.y = GetScreenHeight() - image.height;
}
