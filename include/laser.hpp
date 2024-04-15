#pragma once
#include <raylib.h>

class Laser
{
public:
    Laser(Vector2 position, int speed, int type);
    ~Laser();
    
    void Update();
    void Draw();
    bool active;
    Rectangle GetRect();

private:
    Vector2 position;
    int speed;
    Color color;
};

