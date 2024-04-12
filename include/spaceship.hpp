#pragma once
#include <raylib.h>

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

private:
    Texture2D image;
    Vector2 position;
    Vector2 speed;
};
