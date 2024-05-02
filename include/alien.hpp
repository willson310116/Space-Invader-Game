#pragma once
#include <raylib.h>
#include <yaml-cpp/yaml.h>

class Alien
{
public:
    Alien(YAML::Node& config, int type, Vector2 position, float scale);
    void Draw();
    void Update(int direction);
    int GetType();
    Rectangle GetRect();

    static Texture2D alienImages[3];
    static void UnloadImages();

    Vector2 position;
    int type;
    float scale;

private:
};

