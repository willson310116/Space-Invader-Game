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

    bool run;       // whether game is currently running
    int lives;      // player's live
    int score;      // player's score
    int highScore;  // history high score
    Music music;

private:
    Spaceship spaceship;
    std::vector<Obstacle> obstacles;
    
    // aliens
    int aliensDirection;
    std::vector<Alien> aliens;
    std::vector<Laser> alienLasers;
    float timeLastAlienFired;
    constexpr static float alienFireInterval = 0.35;
    
    // mysteryship
    Mysteryship mysteryship;
    float timeLastSpawn;
    float mysteryshipSpawnInterval;

private:
    // general
    void InitGame();
    void Reset();
    void GameOver();
    void DeleteInactiveLaser();
    void CheckCollisions();
    void CheckHighScore();
    void SaveHighScoreToFile(int score);
    int LoadHighScoreFromFile();
    Sound explosionSound;
    
    // alien
    void AlienFire();
    void MoveAliens();
    void MoveDownAliens(int distance);

    // mysteryship
    void SpawnMystership();
    
    // creating stuff
    std::vector<Alien> CreateAliens();
    std::vector<Obstacle> CreateObstacles(int numObstacles);
    
    // scoring
    void AddScore(std::vector<Alien>::iterator it);
    void GetMyteryshipReward();
};


