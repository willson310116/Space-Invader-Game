#include "spaceship.hpp"

Spaceship::Spaceship()
{
    image = LoadTexture("../asset/graphics/spaceship.png");
    position.x = (GetScreenWidth() - image.width) / 2;
    position.y = GetScreenHeight() - image.height - 100;
    speed.x = 7;
    speed.y = 7;
    lastFireTime = 0.0;
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
    if (position.x < 25)
        position.x = 25;
}

void Spaceship::MoveRight()
{
    position.x += speed.x;
    if (position.x > GetScreenWidth() - image.width - 25)
        position.x = GetScreenWidth() - image.width - 25;
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

void Spaceship::Fire()
{
    // add delay between fires
    if (GetTime() - lastFireTime >= 0.35)
    {
        // the laser has 4 pixel of width, so minus 2 to centerize
        // lasers.push_back(
        //     Laser({position.x + image.width/2 - 2, position.y}, -6));
        lasers.emplace_back(
            Vector2{position.x + image.width/2 - 2, position.y}, -6, 1);
        lastFireTime = GetTime();
    }
}

Rectangle Spaceship::GetRect()
{
    return {position.x, position.y, static_cast<float>(image.width), static_cast<float>(image.height)};
}

void Spaceship::Reset()
{
    position.x = (GetScreenWidth() - image.width) / 2;
    position.y = GetScreenHeight() - image.height - 100;
    // speed.x = 7;
    // speed.y = 7;
    // lastFireTime = 0.0;
    lasers.clear();
}
