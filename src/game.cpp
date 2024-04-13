#include <iostream>
#include "game.hpp"

Game::Game()
{
    obstacles = CreateObstacles(5);
    aliens = CreateAliens();
    aliensDirection = 1;
    timeLastAlienFired = 0;
    timeLastSpawn = 0;
    mysteryshipSpawnInterval = GetRandomValue(10, 20);
}

Game::~Game()
{
    Alien::UnloadImages();   
}

void Game::Update()
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
    if (IsKeyDown(KEY_LEFT))
        spaceship.MoveLeft();
    else if (IsKeyDown(KEY_RIGHT))
        spaceship.MoveRight();
    else if (IsKeyDown(KEY_UP))
        spaceship.MoveUp();
    else if (IsKeyDown(KEY_DOWN))
        spaceship.MoveDown();
    else if (IsKeyDown(KEY_SPACE))
        spaceship.Fire();
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
        obstacles.emplace_back(Vector2{offsetX, static_cast<float>(GetScreenHeight() - 100)});
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
        if (alien.position.x + alien.alienImages[alien.type - 1].width > GetScreenWidth())
        {
            aliensDirection = -1;
            MoveDownAliens(4);
        }
            
        else if (alien.position.x < 0)
        {
            aliensDirection = 1;
            MoveDownAliens(4);
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
