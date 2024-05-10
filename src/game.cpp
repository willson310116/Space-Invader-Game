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
    bossMusicPath = node["BossMusic"].as<std::string>();
    explosionSoundPath = node["ExplosionSound"].as<std::string>();
    bossWarningSoundPath = node["BossWarningSound"].as<std::string>();
    fontPath = node["Font"].as<std::string>();
    spaceshipImgPath = node["SpaceShipImg"].as<std::string>();
    highScoreFile = node["HighScore"].as<std::string>();

    // general
    // YAML::Node is ref type
    YAML::Node node1 = config["Game"]["General"];
    musicVolume = node1["MusicVolume"].as<float>();
    initLives = node1["Lives"].as<int>();
    rewardDisplayInterval = node1["RewardDisplayInterval"].as<float>();
    bossMenuDisplayInterval = node1["BossMenuDisplayInterval"].as<float>();
    
    // reward
    YAML::Node node2 = config["Game"]["Reward"];
    if (node2["Alien"].IsSequence())
    {
        for (auto score : node2["Alien"])
            alienReward.push_back(score.as<int>());
    }
    mysteryship.reward.addscore = node2["MysteryShip"]["AddScore"].as<int>();
    mysteryship.reward.addMove = node2["MysteryShip"]["AddMove"].as<int>();
    mysteryship.reward.addLive = node2["MysteryShip"]["AddLive"].as<int>();
    mysteryship.reward.reduceFireInterval = node2["MysteryShip"]["ReduceFireIntervalFactor"].as<float>();
    mysteryship.reward.addLaserSpeed = node2["MysteryShip"]["AddLaserSpeed"].as<int>();
}

void Game::LoadLevelConfig()
{
    std::string curLevel = "LEVEL" + std::to_string(mainMenu.level);
    YAML::Node node = config["Game"][curLevel];
    
    // obstacle
    numObstacles = node["Obstacle"]["NumObstacles"].as<int>();

    // alien
    alienRows = node["Alien"]["Rows"].as<int>();
    alienCols = node["Alien"]["Cols"].as<int>();
    alienSpeed = node["Alien"]["Speed"].as<int>();
    alienDropDistance = node["Alien"]["DropDistance"].as<int>();
    alienFireInterval = node["Alien"]["FireInterval"].as<float>();
    alienLaserSpeed = node["Alien"]["LaserSpeed"].as<int>();
    
    // mysteryship
    mysteryshipSpawnIntervalLowerBound = node["MysteryShip"]["SpawnIntervalLowerBound"].as<int>();
    mysteryshipSpawnIntervalUpperBound = node["MysteryShip"]["SpawnIntervalUpperBound"].as<int>();
  
    // boss
    initBossLives = node["Boss"]["Live"].as<int>();
    bossSpeed = node["Boss"]["Speed"].as<int>();
    bossScale = node["Boss"]["Scale"].as<float>();
    bossFireInterval = node["Boss"]["FireInterval"].as<float>();
    bossLaserSpeed = node["Boss"]["LaserSpeed"].as<int>();
}

void Game::Debugger(const char* text)
{
    std::cout << text << std::endl;
}

Game::Game(YAML::Node& config) :
    config(config), spaceship(config), mysteryship(config), optionList(config), mainMenu(config), bossMenu(config)
{
    SetParams();
    music = LoadMusicStream(backgroundMusicPath.c_str());
    bossMusic = LoadMusicStream(bossMusicPath.c_str());
    explosionSound = LoadSound(explosionSoundPath.c_str());
    bossWarningSound = LoadSound(bossWarningSoundPath.c_str());
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

    if (loadBossFlag)
    {
        InitBossStage();
        mysteryship.alive = false;
        loadBossFlag = false;
    }
}

void Game::InitBossStage()
{
    float x = 200, y = 200;
    bosses.emplace_back(config, 4, Vector2{x, y}, bossScale);
    bossLive = initBossLives;
    timeLastBossFired = 0;
    // timeLastDisplayBossMenu = GetTime();
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
    if (bossMenu.active)
    {
        // Debugger("bossMenu is active");
        double curTime = GetTime();
        if (curTime - timeLastDisplayBossMenu <= bossMenuDisplayInterval)
        {
            displayBossMenuFlag = true;
        }
        else
        {
            bossMenu.active = false;
            displayBossMenuFlag = false;
            music = bossMusic;
            PlayMusicStream(music);
        }
        
    }
    // dont run anything if gameover
    else if (run)
    {
        spaceship.Update();
        AlienFire();
        MoveAliens();
        if (!bossStateStart)
        {
            SpawnMystership();
            mysteryship.Update();
            for (auto& laser : alienLasers) laser.Update();   
            if (aliens.size() == 0)
            {
                StartBossState();
                PlaySound(bossWarningSound);
                StopMusicStream(music);
            }
        }
        else
        {
            for (auto& laser : bossLasers) laser.Update();   
            if (bosses.size() == 0)
            {
                bossStateStart = false;
                GameOver();
            }
        }
        DeleteInactiveLaser();
        CheckCollisions();
    }
    else
    {
        optionList.Update();
        if (optionList.buttons[optionList.RESTART].IsPressed())
        {
            Reset();
            loadFlag = true;
            bossStateStart = false;
        }

        else if (optionList.buttons[optionList.NEXT].IsPressed())
        {
            Reset();
            if (mainMenu.level < 3)
                mainMenu.level += 1;
            loadFlag = true;
            bossStateStart = false;
        }

        else if (optionList.buttons[optionList.EXIT].IsPressed())
        {
            curGameState = GameState::EXIT;
        }
        
        else if (optionList.buttons[optionList.MENU].IsPressed())
        {
            curGameState = GameState::MENU;
            Reset();
            loadFlag = true;
        }
    }
    
}

void Game::Draw()
{
    DrawLayout();
    spaceship.Draw();
    for (auto& obstacle : obstacles) obstacle.Draw();

    if (displayBossMenuFlag)
    {
        bossMenu.Draw();
    }      
    
    else if (!bossStateStart && run)
    {
        for (auto& alien : aliens) alien.Draw();
        for (auto& laser : alienLasers) laser.Draw();
        for (auto& laser : spaceship.lasers) laser.Draw();
        mysteryship.Draw();
        DisplayMysteryshipReward();
    }
    else if (bossStateStart)
    {
        DisplayBossLive();
        for (auto& boss : bosses) boss.Draw();
        for (auto& laser : bossLasers) laser.Draw();
        for (auto& laser : spaceship.lasers) laser.Draw();
    }
    
    if (!run) optionList.Draw();
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

    if (!bossStateStart)
    {
        alienLasers.erase(
            std::remove_if(
                alienLasers.begin(), 
                alienLasers.end(),
                [](Laser const & laser) { return !laser.active; }
            ), 
            alienLasers.end()
        );
    }
    else
    {
        bossLasers.erase(
            std::remove_if(
                bossLasers.begin(), 
                bossLasers.end(),
                [](Laser const & laser) { return !laser.active; }
            ), 
            bossLasers.end()
        );
    }
    
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
        int alienType;
            if (i == 0)
                alienType = 3;
            else if (i == 1 || i == 2)
                alienType = 2;
            else
                alienType = 1;
        for (int j = 0; j < col; j++)
        {
            float x = offsetX + j * gapBetweenAliens;
            float y = offsetY + i * gapBetweenAliens;
            aliens.emplace_back(config, alienType, Vector2{x, y});
        }
    }
    return aliens;
}

void Game::MoveAliensHelper(std::vector<Alien>& aliens_)
{
    for (auto& alien : aliens_)
    {
        if (alien.position.x + alien.alienImages[alien.type - 1].width * alien.scale > GetScreenWidth() - 25)
        {
            if (alienSpeed > 0)
                alienSpeed *= -1;
            MoveDownAliens(aliens_);
        }
            
        else if (alien.position.x < 25)
        {
            if (alienSpeed < 0)
                alienSpeed *= -1;
            MoveDownAliens(aliens_);
        }
            
        alien.Update(alienSpeed);
    }
}

void Game::MoveAliens()
{
    MoveAliensHelper(aliens);
    MoveAliensHelper(bosses);
}

void Game::MoveDownAliens(std::vector<Alien>& aliens_)
{
    for (auto& alien : aliens_)
        alien.position.y += alienDropDistance;
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
    if (!bossStateStart)
    {
        // normal alien
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
    else
    {
        // boss alien
        if (curTime - timeLastBossFired >= bossFireInterval && !bosses.empty())
        {
            int randomIndex = GetRandomValue(0, bosses.size() - 1);
            Alien& boss = bosses[randomIndex];
            
            bossLasers.emplace_back(
                Vector2{boss.position.x + boss.alienImages[boss.type - 1].width / 2 * boss.scale,
                    boss.position.y + boss.alienImages[boss.type - 1].height * boss.scale}, bossLaserSpeed, 3);
            
            timeLastBossFired = GetTime();
        }
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

        it = bosses.begin();
        while (it != bosses.end())
        {
            if (CheckCollisionRecs(it->GetRect(), laser.GetRect()))
            {
                PlaySound(explosionSound);
                AddScore(it);
                CheckHighScore();
                laser.active = false;
                bossLive -= 1;
                if (bossLive <= 0)
                    it = bosses.erase(it);
                else
                    it += 1;
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

    // boss lasers
    for (auto& laser : bossLasers)
    {
        if (CheckCollisionRecs(laser.GetRect(), spaceship.GetRect()))
        {
            laser.active = false;
            lives -= 1;
            if (lives == 0)
                GameOver();
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

void Game::DisplayBossLive()
{
    DrawRectangleRoundedLines({50, 120, float(GetScreenWidth()-100), 7}, 0.18f, 20, 2, WHITE);
    DrawRectangleRec({50, 120,
        float((GetScreenWidth()-100) * bossLive / initBossLives), 7}, RED);
}

void Game::GameOver()
{
    run = false;
}

void Game::StartBossState()
{
    bossStateStart = true;
    loadBossFlag = true;
    bossMenu.active = true;
    timeLastDisplayBossMenu = GetTime();
}

void Game::Reset()
{
    spaceship.Reset();
    aliens.clear();
    alienLasers.clear();
    obstacles.clear();
    bosses.clear();
    bossLasers.clear();
}

void Game::AddScore(std::vector<Alien>::iterator it)
{
    if (it->type > 0 && it->type <= alienReward.size())
        score += alienReward[it->type-1];
}

void Game::GetMysteryshipReward()
{
    int rewardNum = GetRandomValue(1, 5);
    timeLastDisplayReward = GetTime();
    switch (rewardNum)
    {
    case 1:
        score += mysteryship.reward.addscore;
        break;
    case 2:
        spaceship.speed.x += mysteryship.reward.addMove;
        break;
    case 3:
        lives += mysteryship.reward.addLive;
        break;
    case 4:
        spaceship.fireInterval /= mysteryship.reward.addLaserSpeed;
        break;
    case 5:
        spaceship.laserSpeed -= mysteryship.reward.addLaserSpeed;
        break;
    }
    rewardState = static_cast<RewardState>(rewardNum);
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

