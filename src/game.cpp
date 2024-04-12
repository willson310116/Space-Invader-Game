#include <iostream>
#include "game.hpp"

Game::Game(/* args */)
{

}

Game::~Game()
{
    
}

void Game::Update()
{
    for (auto& laser : spaceship.lasers)
    {
        laser.Update();
    }

    DeleteInactiveLaser();
    // std::cout << "vector size: " << spaceship.lasers.size() << std::endl;
}

void Game::Draw()
{
    spaceship.Draw();
    for (auto& laser : spaceship.lasers)
    {
        laser.Draw();
    }
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