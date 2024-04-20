#pragma once
#include "MainMenu.hpp"
#include "Alien.hpp"
#include "Obstacle.hpp"
#include "SpaceShip.hpp"
#include "OptionList.hpp"
#include "MysteryShip.hpp"

std::string FormatWithLeadingZeros(int number, int width);
extern GameState curGameState;

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
    
    MainMenu mainMenu;
    OptionList optionList;
    
    Font font = LoadFontEx("../asset/font/monogram.ttf", 64, 0, 0);
    Texture2D spaceshipImage = LoadTexture("../asset/graphics/spaceship.png");

private:
    SpaceShip spaceship;
    std::vector<Obstacle> obstacles;
    
    // aliens
    int aliensDirection;
    std::vector<Alien> aliens;
    std::vector<Laser> alienLasers;
    float timeLastAlienFired;
    constexpr static float alienFireInterval = 0.5;
    
    // mysteryship
    MysteryShip mysteryship;
    float timeLastSpawn;
    float mysteryshipSpawnInterval;
    float timeLastDisplayReward;
    float rewardDisplayInterval;
    RewardState rewardState;

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
    std::vector<Alien> CreateAliens(int row, int col);
    std::vector<Obstacle> CreateObstacles(int numObstacles);
    
    // scoring
    void AddScore(std::vector<Alien>::iterator it);
    void GetMysteryshipReward();
    void DisplayMysteryshipReward();
};


