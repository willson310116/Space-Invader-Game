#pragma once
#include <raylib.h>

class Alien
{
public:
    Alien(int type, Vector2 position);
    void Draw();
    void Update(int direction);
    int GetType();
    Rectangle GetRect();

    static Texture2D alienImages[3];
    static void UnloadImages();

    Vector2 position;
    int type;

private:
};

