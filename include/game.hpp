#pragma once
#include "spaceship.hpp"
#include "obstacle.hpp"
#include "alien.hpp"
#include "mysteryship.hpp"

class Game
{
public:
    Game();
    ~Game();

    void Draw();
    void Update();
    void HandleInput();

    bool run;
    int lives;
    int score;

private:
    Spaceship spaceship;
    void DeleteInactiveLaser();
    std::vector<Obstacle> obstacles;
    std::vector<Obstacle> CreateObstacles(int numObstacles);
    std::vector<Alien> aliens;
    std::vector<Alien> CreateAliens();
    void MoveAliens();
    int aliensDirection;
    void MoveDownAliens(int distance);
    std::vector<Laser> alienLasers;
    void AlienFire();
    constexpr static float alienFireInterval = 0.35;
    float timeLastAlienFired;
    Mysteryship mysteryship;
    float mysteryshipSpawnInterval;
    float timeLastSpawn;
    void CheckCollisions();
    void GameOver();
    void Reset();
    void InitGame();
    void AddScore(std::vector<Alien>::iterator it);
    void GetMyteryshipReward();
};


