#pragma once
#include <yaml-cpp/yaml.h>
#include "MainMenu.hpp"
#include "Alien.hpp"
#include "Obstacle.hpp"
#include "SpaceShip.hpp"
#include "OptionList.hpp"
#include "MysteryShip.hpp"

std::string FormatWithLeadingZeros(int number, int width);
extern GameState curGameState;
extern bool loadFlag;

class Game
{
public:
    Game(YAML::Node& config);
    ~Game();

    void Draw();
    void Update();
    void HandleInput();
    void SetGame();

    bool run;       // whether game is currently running
    int lives;      // player's live
    int initLives;
    int score;      // player's score
    int highScore;  // history high score
    Color backgroundColor = {29, 29, 29, 255};
    Music music;
    Sound explosionSound;
    
    MainMenu mainMenu;
    OptionList optionList;
    
    Font font;
    Texture2D spaceshipImage;

private:
    // general
    YAML::Node config;
    std::string backgroundMusicPath;
    std::string explosionSoundPath;
    std::string fontPath;
    std::string spaceshipImgPath;
    std::string highScoreFile;
    float musicVolume;
    
    // spaceship
    SpaceShip spaceship;
    
    // obstacle
    int numObstacles;
    std::vector<Obstacle> obstacles;
    
    // aliens
    int alienSpeed;
    int alienRows;
    int alienCols;
    int alienLaserSpeed;
    int alienDropDistance;
    std::vector<Alien> aliens;
    std::vector<Laser> alienLasers;
    float timeLastAlienFired;
    float alienFireInterval;
    
    // mysteryship
    MysteryShip mysteryship;
    float timeLastSpawn;
    int mysteryshipSpawnIntervalLowerBound;
    int mysteryshipSpawnIntervalUpperBound;
    int mysteryshipsSpeed;
    float mysteryshipSpawnInterval;
    float timeLastDisplayReward;
    float rewardDisplayInterval;
    RewardState rewardState;

    // scoring
    std::vector<int> alienReward;    
    // MysteryShipReward mysteryShipReward;

private:
    // general
    void InitGame();
    void SetParams();
    void Reset();
    void GameOver();
    void DeleteInactiveLaser();
    void CheckCollisions();
    void CheckHighScore();
    void SaveHighScoreToFile(int score);
    int LoadHighScoreFromFile();
    void DrawLayout();
    void LoadLevelConfig();
    void Debugger(const char* text);
    
    // alien
    void AlienFire();
    void MoveAliens();
    void MoveDownAliens(int distance);
    std::vector<Alien> CreateAliens(int row, int col);

    // mysteryship
    void SpawnMystership();
    
    // obstacle
    std::vector<Obstacle> CreateObstacles(int numObstacles);
    
    // scoring
    void AddScore(std::vector<Alien>::iterator it);
    void GetMysteryshipReward();
    void DisplayMysteryshipReward();
};


