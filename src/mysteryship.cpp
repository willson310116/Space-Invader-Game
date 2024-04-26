#include "MysteryShip.hpp"
#include <iostream>

MysteryShip::MysteryShip(YAML::Node& config)
{
    image = LoadTexture(config["Path"]["MysteryShipImg"].as<std::string>().c_str());
    alive = false;
    speed = config["Game"]["MysteryShip"]["Speed"].as<int>();
}

MysteryShip::~MysteryShip()
{
    UnloadTexture(image);
}

void MysteryShip::Spawn()
{
    position.y = 80;
    int side = GetRandomValue(0, 1);

    if (side == 0)
    {
        position.x = 25;
        if (speed < 0)
            speed *= -1;
    }
    else
    {
        position.x = GetScreenWidth() - image.width - 25;
        if (speed > 0)
            speed *= -1;
    }
    alive = true;
}

Rectangle MysteryShip::GetRect()
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

void MysteryShip::Update()
{
    if (alive)
    {
        position.x += speed;
        if (position.x > GetScreenWidth() - image.width - 25 || position.x < 25)
            alive = false;
    }
}

void MysteryShip::Draw()
{
    if (alive)
        DrawTextureV(image, position, WHITE);
}