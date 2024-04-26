#include "Alien.hpp"
#include <iostream>

Texture2D Alien::alienImages[3] = {};

Alien::Alien(YAML::Node& config, int type, Vector2 position)
    : type(type), position(position)
{
    if (alienImages[type - 1].id == 0)
    {
        switch (type)
        {
        case 1:
            alienImages[0] = LoadTexture(config["Path"]["Alien_1_Img"].as<std::string>().c_str());
            break;
        case 2:
            alienImages[1] = LoadTexture(config["Path"]["Alien_2_Img"].as<std::string>().c_str());
            break;
        case 3:
            alienImages[2] = LoadTexture(config["Path"]["Alien_3_Img"].as<std::string>().c_str());
            break;
        default:
            alienImages[0] = LoadTexture(config["Path"]["Alien_1_Img"].as<std::string>().c_str());
            break;
        }
    }
}

void Alien::Draw()
{
    DrawTextureV(alienImages[type - 1], position, WHITE);
}

int Alien::GetType()
{
    return type;
}

void Alien::UnloadImages()
{
    for (int i = 0; i < 3; i++)
        UnloadTexture(alienImages[i]);
}

void Alien::Update(int direction)
{
    position.x += direction;
    
}

Rectangle Alien::GetRect()
{
    return {position.x, position.y,
            static_cast<float>(alienImages[type-1].width),
            static_cast<float>(alienImages[type-1].height)};
}
