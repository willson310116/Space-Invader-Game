#include <iostream>
#include <fstream>
#include <string>
#include "Game.hpp"
#include "ButtonHandler.hpp"

std::string FormatWithLeadingZeros(int number, int width)
{
    std::string numberText = std::to_string(number);
    int leadingZeros = width - numberText.size();
    return std::string(leadingZeros, '0') + numberText;
}

void Game::SetParams()
{
    // path
    YAML::Node node = config["Path"];
    backgroundMusicPath = node["BackgroundMusic"].as<std::string>();
    explosionSoundPath = node["ExplosionSound"].as<std::string>();
    fontPath = node["Font"].as<std::string>();
    spaceshipImgPath = node["SpaceShipImg"].as<std::string>();
    highScoreFile = node["HighScore"].as<std::string>();

    // general
    // YAML::Node is ref type
    YAML::Node node1 = config["Game"]["General"];
    musicVolume = node1["MusicVolume"].as<float>();
    initLives = node1["Lives"].as<int>();
    rewardDisplayInterval = node1["RewardDisplayInterval"].as<float>();
}

void Game::LoadLevelConfig()
{
    std::string curLevel = "LEVEL" + std::to_string(mainMenu.level);
    YAML::Node node = config["Game"][curLevel];
    
    // obstacle
    numObstacles = node["Obstacle"]["NumObstacles"].as<int>();

    // alien
    alienRows = node["Alien"]["AlienRows"].as<int>();
    alienCols = node["Alien"]["AlienCols"].as<int>();
    alienSpeed = node["Alien"]["AlienSpeed"].as<int>();
    alienDropDistance = node["Alien"]["AlienDropDistance"].as<int>();
    alienFireInterval = node["Alien"]["FireInterval"].as<float>();
    
    // mysteryship
    mysteryshipSpawnIntervalLowerBound = node["MysteryShip"]["MysteryshipSpawnIntervalLowerBound"].as<int>();
    mysteryshipSpawnIntervalUpperBound = node["MysteryShip"]["MysteryshipSpawnIntervalUpperBound"].as<int>();
  
    // alien
    alienLaserSpeed = node["Laser"]["AlienLaserSpeed"].as<int>();
}

void Game::Debugger(const char* text)
{
    std::cout << text << std::endl;
}

Game::Game(YAML::Node& config) :
    config(config), spaceship(config), mysteryship(config), optionList(config), mainMenu(config)
{
    SetParams();
    music = LoadMusicStream(backgroundMusicPath.c_str());
    explosionSound = LoadSound(explosionSoundPath.c_str());
    PlayMusicStream(music);
    SetMusicVolume(music, musicVolume);
    font = LoadFontEx(fontPath.c_str(), 64, 0, 0);
    spaceshipImage = LoadTexture(spaceshipImgPath.c_str());
}

void Game::SetGame()
{
    if (loadFlag)
    {
        LoadLevelConfig();
        InitGame();
        loadFlag = false;
    }
}

void Game::InitGame()
{
    obstacles = CreateObstacles(numObstacles);
    aliens = CreateAliens(alienRows, alienCols);
    timeLastAlienFired = 0;
    timeLastSpawn = 0;
    mysteryshipSpawnInterval = GetRandomValue(
        mysteryshipSpawnIntervalLowerBound, mysteryshipSpawnIntervalUpperBound);
    
    run = true;
    score = 0;
    highScore = LoadHighScoreFromFile();
    timeLastDisplayReward = 0;
    rewardState = RewardState::NONE;
    lives = initLives;
}


Game::~Game()
{
    Alien::UnloadImages();   
    UnloadMusicStream(music);
    UnloadSound(explosionSound);
}

void Game::Update()
{
    // dont run anything if gameover
    if (run)
    {
        SpawnMystership();
        spaceship.Update();
        mysteryship.Update();
        AlienFire();
        for (auto& laser : alienLasers) laser.Update();
        
        MoveAliens();
        DeleteInactiveLaser();
        CheckCollisions();
        if (aliens.size() == 0) GameOver();
    }
    else
    {
        optionList.Update();
        if (optionList.buttons[optionList.RESTART].IsPressed())
        {
            Reset();
            loadFlag = true;
        }

        else if (optionList.buttons[optionList.NEXT].IsPressed())
        {
            Reset();
            if (mainMenu.level < 3)
                mainMenu.level += 1;
            loadFlag = true;
        }

        else if (optionList.buttons[optionList.EXIT].IsPressed())
        {
            curGameState = GameState::EXIT;
        }
        
        else if (optionList.buttons[optionList.MENU].IsPressed())
        {
            curGameState = GameState::MENU;
            Reset();
            // InitGame();
            loadFlag = true;
        }
    }
    
}

void Game::Draw()
{
    DrawLayout();
    DisplayMysteryshipReward();
    spaceship.Draw();
    for (auto& laser : spaceship.lasers)
        laser.Draw();
    for (auto& obstacle : obstacles)
        obstacle.Draw();
    for (auto& alien : aliens)
        alien.Draw();
    for (auto& laser : alienLasers)
        laser.Draw();
    mysteryship.Draw();
    
    if (!run)
        optionList.Draw();

}

void Game::HandleInput()
{
    if (run)
    {
        if (IsKeyDown(KEY_LEFT))
            spaceship.MoveLeft();
        else if (IsKeyDown(KEY_RIGHT))
            spaceship.MoveRight();
        else if (IsKeyDown(KEY_SPACE))
            spaceship.Fire();
    } 
}

// limit the amount of lasers in laser vector
void Game::DeleteInactiveLaser()
{
    //  Erase-Remove Idiom
    spaceship.lasers.erase(
        std::remove_if(
            spaceship.lasers.begin(), 
            spaceship.lasers.end(),
            [](Laser const & laser) { return !laser.active; }
        ), 
        spaceship.lasers.end()
    ); 

    alienLasers.erase(
        std::remove_if(
            alienLasers.begin(), 
            alienLasers.end(),
            [](Laser const & laser) { return !laser.active; }
        ), 
        alienLasers.end()
    ); 

    // [warning] good, but avoid using raw for-loop
    // for (auto it = spaceship.lasers.begin(); it != spaceship.lasers.end();)
    // {
    //     if (!it->active)
    //         it = spaceship.lasers.erase(it);
    //     else
    //         it += 1;
    // }

    // [warning] the size of the vector will change, dont use
    // for (int i = 0; i < spaceship.lasers.size(); i++)
    // {
    //     if (!spaceship.lasers[i].active)
    //         spaceship.lasers.erase(spaceship.lasers.begin()+i);
    // }
}

std::vector<Obstacle> Game::CreateObstacles(int numObstacles)
{
    int obstacleWidth = Obstacle::grid[0].size() * 3;
    float gapBetweenObstacles = (GetScreenWidth() - numObstacles * obstacleWidth) / (numObstacles + 1);
    for (int i = 0; i < numObstacles; i++)
    {
        float offsetX = (i + 1) * gapBetweenObstacles + i * obstacleWidth;
        obstacles.emplace_back(
            Vector2{offsetX, static_cast<float>(GetScreenHeight() - 200)});
    }
    return obstacles;
}

std::vector<Alien> Game::CreateAliens(int row, int col)
{
    std::vector<Alien> aliens;
    int gapBetweenAliens = 55;
    int offsetX = 75;
    int offsetY = 110;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            int alienType;
            if (i == 0)
                alienType = 3;
            else if (i == 1 || i == 2)
                alienType = 2;
            else
                alienType = 1;

            float x = offsetX + j * gapBetweenAliens;
            float y = offsetY + i * gapBetweenAliens;
            aliens.emplace_back(config, alienType, Vector2{x, y});
        }
    }
    return aliens;
}

void Game::MoveAliens()
{
    for (auto& alien : aliens)
    {
        if (alien.position.x + alien.alienImages[alien.type - 1].width > GetScreenWidth() - 25)
        {
            if (alienSpeed > 0)
                alienSpeed *= -1;
            MoveDownAliens(alienDropDistance);
        }
            
        else if (alien.position.x < 25)
        {
            if (alienSpeed < 0)
                alienSpeed *= -1;
            MoveDownAliens(alienDropDistance);
        }
            
        alien.Update(alienSpeed);
    }
}

void Game::MoveDownAliens(int distance)
{
    for (auto& alien : aliens)
    {
        alien.position.y += distance;
    }
}

void Game::SpawnMystership()
{
    double curTime = GetTime();
    if (!mysteryship.alive && curTime - timeLastSpawn > mysteryshipSpawnInterval)
    {
        mysteryship.Spawn();
        timeLastSpawn = GetTime();
        mysteryshipSpawnInterval = GetRandomValue(
            mysteryshipSpawnIntervalLowerBound, mysteryshipSpawnIntervalUpperBound);
    }
}

void Game::AlienFire()
{
    double curTime = GetTime();
    if (curTime - timeLastAlienFired >= alienFireInterval && !aliens.empty())
    {
        int randomIndex = GetRandomValue(0, aliens.size() - 1);
        Alien& alien = aliens[randomIndex];
        
        alienLasers.emplace_back(
            Vector2{alien.position.x + alien.alienImages[alien.type - 1].width / 2,
                alien.position.y + alien.alienImages[alien.type - 1].height}, alienLaserSpeed, 2);
        
        timeLastAlienFired = GetTime();
    }
}

void Game::CheckCollisions()
{
    // spaceship lasers
    for (auto& laser : spaceship.lasers)
    {
        auto it = aliens.begin();
        while (it != aliens.end())
        {
            if (CheckCollisionRecs(it->GetRect(), laser.GetRect()))
            {
                PlaySound(explosionSound);
                AddScore(it);
                CheckHighScore();
                it = aliens.erase(it);
                laser.active = false;
            }
            else
                it += 1;   
        }

        for (auto& obstacle : obstacles)
        {
            auto it = obstacle.blocks.begin();
            while (it != obstacle.blocks.end())
            {
                if (CheckCollisionRecs(it->GetRect(), laser.GetRect()))
                {
                    it = obstacle.blocks.erase(it);
                    laser.active = false;
                }
                else
                    it += 1;
            }
        }

        if (mysteryship.alive && CheckCollisionRecs(mysteryship.GetRect(), laser.GetRect()))
        {
            PlaySound(explosionSound);
            mysteryship.alive = false;
            laser.active = false;
            GetMysteryshipReward();
            CheckHighScore();
        }
    }

    // alien lasers
    for (auto& laser : alienLasers)
    {
        if (CheckCollisionRecs(laser.GetRect(), spaceship.GetRect()))
        {
            laser.active = false;
            lives -= 1;
            if (lives == 0)
                GameOver();
        }

        for (auto& obstacle : obstacles)
        {
            auto it = obstacle.blocks.begin();
            while (it != obstacle.blocks.end())
            {
                if (CheckCollisionRecs(it->GetRect(), laser.GetRect()))
                {
                    it = obstacle.blocks.erase(it);
                    laser.active = false;
                }
                else
                    it += 1;
            }
        }
    }

    // alien collide with obstacle
    for (auto& alien : aliens)
    {
        for (auto& obstacle : obstacles)
        {
            auto it = obstacle.blocks.begin();
            while (it != obstacle.blocks.end())
            {
                if (CheckCollisionRecs(it->GetRect(), alien.GetRect()))
                    it = obstacle.blocks.erase(it);
                else
                    it += 1;
            }
        }
        if (CheckCollisionRecs(alien.GetRect(), spaceship.GetRect()))
        {
            GameOver();
        }
            
    }
}

void Game::CheckHighScore()
{
    if (score > highScore)
        highScore = score;
    SaveHighScoreToFile(highScore);
}

void Game::SaveHighScoreToFile(int score)
{
    std::ofstream file("highscore.txt");
    if (file.is_open())
    {
        file << score;
        file.close();
    }
    else
        std::cerr << "Failed to save highscore to file!" << std::endl;
}

int Game::LoadHighScoreFromFile()
{
    int loadedScore = 0;
    std::ifstream file(highScoreFile);
    if (file.is_open())
    {
        file >> loadedScore;
        file.close();
    }
    else
        std::cerr << "Failed to load highscore from file!" << std::endl;
    
    return loadedScore;
}

void Game::DrawLayout()
{
    DrawRectangleRoundedLines({10, 10, 780, 780}, 0.18f, 20, 2, YELLOW);
    DrawLineEx({25, 730}, {775, 730}, 3, YELLOW);
    std::string levelText = "LEVEL 0" + std::to_string(mainMenu.level);
    
    if (run)
        DrawTextEx(font, levelText.c_str(), {570, 740}, 34, 2, YELLOW);
    else
    {
        if (aliens.size() == 0)
            DrawTextEx(font, "YOU WON", {570, 740}, 34, 2, YELLOW);
        else
            DrawTextEx(font, "GAME OVER", {570, 740}, 34, 2, YELLOW);
    }

    DrawTextEx(font, "LIVES", {70, 740}, 34, 2, YELLOW);
    float x = 180.0;
    for (int i = 0; i < lives; i++)
    {
        DrawTextureV(spaceshipImage, {x, 740}, WHITE);
        x += 50;
    }
    DrawTextEx(font, "SCORE", {50, 15}, 36, 2, YELLOW);
    std::string scoreText = FormatWithLeadingZeros(score, 5);
    DrawTextEx(font, scoreText.c_str(), {50, 40}, 36, 2, YELLOW);

    DrawTextEx(font, "HIGH-SCORE", {570, 15}, 36, 2, YELLOW);
    std::string highScoreText = FormatWithLeadingZeros(highScore, 5);
    DrawTextEx(font, highScoreText.c_str(), {665, 40}, 36, 2, YELLOW);
}

void Game::GameOver()
{
    run = false;
}

void Game::Reset()
{
    spaceship.Reset();
    aliens.clear();
    alienLasers.clear();
    obstacles.clear();
}

void Game::AddScore(std::vector<Alien>::iterator it)
{
    if (it->type == 1)
        score += 100;
    else if (it->type == 2)
        score += 200;
    else if (it->type == 3)
        score += 300;
}

void Game::GetMysteryshipReward()
{
    int rewardNum = GetRandomValue(1, 5);
    timeLastDisplayReward = GetTime();
    switch (rewardNum)
    {
    case 1:
        score += 500;
        rewardState = RewardState::ADD_SCORE;
        break;
    case 2:
        spaceship.speed.x += 2;
        rewardState = RewardState::ADD_MOVE;
        break;
    case 3:
        lives += 1;
        rewardState = RewardState::ADD_LIVE;
        break;
    case 4:
        spaceship.fireInterval /= 2;
        rewardState = RewardState::ADD_LASER;
        break;
    case 5:
        spaceship.laserSpeed -= 2;
        rewardState = RewardState::ADD_LASER_SPEED;
        break;
    default:
        break;
    }
}

void Game::DisplayMysteryshipReward()
{
    double curTime = GetTime();
    if (curTime - timeLastDisplayReward <= rewardDisplayInterval)
    {
        switch (rewardState)
        {
        case RewardState::NONE:
            break;
        case RewardState::ADD_SCORE:
            DrawTextEx(font, "POINT + 500", {200, 15}, 36 , 2, GREEN);
            break;
        case RewardState::ADD_MOVE:
            DrawTextEx(font, "FASTER MOVE", {200, 15}, 36, 2, GREEN);
            break;
        case RewardState::ADD_LIVE:
            DrawTextEx(font, "LIVE + 1", {200, 15}, 36, 2, GREEN);
            break;
        case RewardState::ADD_LASER:
            DrawTextEx(font, "MORE LASER", {200, 15}, 36, 2, GREEN);
            break;
        case RewardState::ADD_LASER_SPEED:
            DrawTextEx(font, "FASTER LASER", {200, 15}, 36, 2, GREEN);
            break;
        default:
            break;
        }
    }
}

