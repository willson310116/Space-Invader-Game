#pragma once
#include <yaml-cpp/yaml.h>
#include "MainMenu.hpp"
#include "Alien.hpp"
#include "Obstacle.hpp"
#include "SpaceShip.hpp"
#include "OptionList.hpp"
#include "MysteryShip.hpp"
#include "BossMenu.hpp"

std::string FormatWithLeadingZeros(int number, int width);
extern GameState curGameState;
extern bool loadFlag;
extern bool loadBossFlag;

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
    Music* music;   // current playing music
    Music backgroundMusic;
    Music bossMusic;
    Sound explosionSound;
    
    MainMenu mainMenu;
    OptionList optionList;
    BossMenu bossMenu;      // stage transition
    Sound bossWarningSound; // stage transition sound
    
    Font font;
    Texture2D spaceshipImage;

private:
    // general
    YAML::Node config;
    std::string backgroundMusicPath;
    std::string bossMusicPath;
    std::string explosionSoundPath;
    std::string bossWarningSoundPath;
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

    // boss
    int bossLive;
    int bossSpeed;
    float bossScale;
    int initBossLives;
    bool bossStateStart = false;
    bool displayBossMenuFlag = false;
    std::vector<Alien> bosses;
    std::vector<Laser> bossLasers;
    int bossLaserSpeed;
    float timeLastBossFired;
    float bossFireInterval;
    float timeLastDisplayBossMenu;
    float bossMenuDisplayInterval;
    
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
    void MoveDownAliens(std::vector<Alien>& aliens_);
    void MoveAliensHelper(std::vector<Alien>& aliens_);
    std::vector<Alien> CreateAliens(int row, int col);

    // boss
    void InitBossStage();
    void LoadBossConfig();
    void DisplayBossLive();
    void StartBossState();

    // mysteryship
    void SpawnMystership();
    
    // obstacle
    std::vector<Obstacle> CreateObstacles(int numObstacles);
    
    // scoring
    void AddScore(std::vector<Alien>::iterator it);
    void GetMysteryshipReward();
    void DisplayMysteryshipReward();
};


