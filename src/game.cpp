#include <iostream>
#include <fstream>
#include "game.hpp"

Game::Game()
{
    InitGame();
}

Game::~Game()
{
    Alien::UnloadImages();   
}

void Game::Update()
{
    // dont run anything if gameover
    if (run)
    {
        double curTime = GetTime();
        if (curTime - timeLastSpawn > mysteryshipSpawnInterval)
        {
            mysteryship.Spawn();
            timeLastSpawn = GetTime();
            mysteryshipSpawnInterval = GetRandomValue(10, 20);
        }

        for (auto& laser : spaceship.lasers)
        {
            laser.Update();
        }
        MoveAliens();
        AlienFire();
        for (auto& laser : alienLasers)
            laser.Update();
        DeleteInactiveLaser();
        // std::cout << "vector size: " << alienLasers.size() << std::endl;
        mysteryship.Update();
        CheckCollisions();
    }
    else
    {
        if (IsKeyDown(KEY_ENTER))
        {
            Reset();
            InitGame();
        }
    }
    
}

void Game::Draw()
{
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
}

void Game::HandleInput()
{
    if (run)
    {
        if (IsKeyDown(KEY_LEFT))
            spaceship.MoveLeft();
        else if (IsKeyDown(KEY_RIGHT))
            spaceship.MoveRight();
        // else if (IsKeyDown(KEY_UP))
        //     spaceship.MoveUp();
        // else if (IsKeyDown(KEY_DOWN))
        //     spaceship.MoveDown();
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
        // obstacles.push_back(
        //     Obstacle({offsetX, static_cast<float>(GetScreenHeight() - 100)}));
        obstacles.emplace_back(Vector2{offsetX, static_cast<float>(GetScreenHeight() - 200)});
    }
    return obstacles;
}

std::vector<Alien> Game::CreateAliens()
{
    std::vector<Alien> aliens;
    int gapBetweenAliens = 55;
    int offsetX = 75;
    int offsetY = 110;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 11; j++)
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
            aliens.push_back(Alien(alienType, {x, y}));
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
            aliensDirection = -1;
            MoveDownAliens(6);
        }
            
        else if (alien.position.x < 25)
        {
            aliensDirection = 1;
            MoveDownAliens(6);
        }
            
        alien.Update(aliensDirection);
    }
}

void Game::MoveDownAliens(int distance)
{
    for (auto& alien : aliens)
    {
        alien.position.y += distance;
    }
}

void Game::AlienFire()
{
    double curTime = GetTime();
    if (curTime - timeLastAlienFired >= alienFireInterval && !aliens.empty())
    {
        int randomIndex = GetRandomValue(0, aliens.size() - 1);
        Alien& alien = aliens[randomIndex];
        
        alienLasers.push_back(
            Laser({alien.position.x + alien.alienImages[alien.type - 1].width / 2,
                alien.position.y + alien.alienImages[alien.type - 1].height}, 6, 2)
        );
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
            mysteryship.alive = false;
            laser.active = false;
            GetMyteryshipReward();
            CheckHighScore();
        }
    }

    // alien lasers
    for (auto& laser : alienLasers)
    {
        if (CheckCollisionRecs(laser.GetRect(), spaceship.GetRect()))
        {
            laser.active = false;
            // std::cout << "Spaceship hit" << std::endl;
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
            // std::cout << "Spaceship get hit by alien" << std::endl;
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
    std::ifstream file("highscore.txt");
    if (file.is_open())
    {
        file >> loadedScore;
        file.close();
    }
    else
        std::cerr << "Failed to load highscore from file!" << std::endl;
    
    return loadedScore;
}

void Game::GameOver()
{
    std::cout << "Game over" << std::endl;
    run = false;
}

void Game::Reset()
{
    spaceship.Reset();
    aliens.clear();
    alienLasers.clear();
    obstacles.clear();
}

void Game::InitGame()
{
    obstacles = CreateObstacles(5);
    aliens = CreateAliens();
    aliensDirection = 1;
    timeLastAlienFired = 0;
    timeLastSpawn = 0;
    mysteryshipSpawnInterval = GetRandomValue(10, 20);
    lives = 3;
    run = true;
    score = 0;
    highScore = LoadHighScoreFromFile();
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

void Game::GetMyteryshipReward()
{
    // TODO: add randomize reward
    score += 500;
}
