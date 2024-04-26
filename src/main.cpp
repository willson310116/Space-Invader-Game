#include <iostream>
#include <raylib.h>
#include "Game.hpp"

GameState curGameState = GameState::MENU;
bool loadFlag = false;

int main() {
    // Load the YAML file
    YAML::Node config = YAML::LoadFile("../asset/config/config.yaml");
    int windowWidth = config["General"]["WindowWidth"].as<int>();
    int windowHeight = config["General"]["WindowHeight"].as<int>();
    int windowOffset = config["General"]["WindowOffset"].as<int>();
    
    InitWindow(windowWidth + windowOffset, windowHeight + 2 * windowOffset, "C++ space invader");
    InitAudioDevice();
    SetTargetFPS(60);
    
    // create instance after window initialization
    Game game(config);
    
    while (WindowShouldClose() == false && curGameState != GameState::EXIT)
    {
        BeginDrawing();
        ClearBackground(game.backgroundColor);
        switch (curGameState)
        {
        case GameState::MENU:
            game.mainMenu.Update();
            game.mainMenu.Draw();
            break;

        case GameState::GAMEPLAY:
            UpdateMusicStream(game.music);
            game.SetGame();
            game.HandleInput();
            game.Update();
            game.Draw();
            break;
        
        default:
            break;
        }
        EndDrawing();
    }

    CloseWindow();
    CloseAudioDevice();
    return 0;
}

