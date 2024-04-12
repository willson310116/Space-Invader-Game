#pragma once
#include "spaceship.hpp"
#include "obstacle.hpp"

class Game
{
public:
    Game();
    ~Game();

    void Draw();
    void Update();
    void HandleInput();

private:
    Spaceship spaceship;
    void DeleteInactiveLaser();
    std::vector<Obstacle> obstacles;
    std::vector<Obstacle> CreateObstacles(int numObstacles);
};

