#include "game.hpp"

Game::Game(/* args */)
{

}

Game::~Game()
{
    
}

void Game::Draw()
{
    spaceship.Draw();
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
}