#include "alien.hpp"

Texture2D Alien::alienImages[3] = {};

Alien::Alien(int type, Vector2 position)
    : type(type), position(position)
{
    if (alienImages[type - 1].id == 0)
    {
        switch (type)
        {
        case 1:
            alienImages[0] = LoadTexture("../asset/graphics/alien_1.png");
            break;
        case 2:
            alienImages[1] = LoadTexture("../asset/graphics/alien_2.png");
            break;
        case 3:
            alienImages[2] = LoadTexture("../asset/graphics/alien_3.png");
            break;
        default:
            alienImages[0] = LoadTexture("../asset/graphics/alien_1.png");
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
