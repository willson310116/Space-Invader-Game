#pragma once
#include "spaceship.hpp"
#include "obstacle.hpp"
#include "alien.hpp"
#include "mysteryship.hpp"

std::string FormatWithLeadingZeros(int number, int width);

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
    Color yellow = {243, 216, 63, 255};
    Color red = {225, 10, 10, 255};
    Font font = LoadFontEx("../asset/font/monogram.ttf", 64, 0, 0);
    Texture2D spaceshipImage = LoadTexture("../asset/graphics/spaceship.png");

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
    void DrawLayout();
    
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


