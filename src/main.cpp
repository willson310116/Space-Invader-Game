#include <raylib.h>
#include "game.hpp"

GameScreen currentScreen = GameScreen::MENU;

int main()
{
    int windowWidth = 750;
    int windowHeight = 700;
    int offset = 50;
    Color grey = {29, 29, 29, 255};

    InitWindow(windowWidth + offset, windowHeight + 2 * offset, "C++ space invader");
    InitAudioDevice();
    SetTargetFPS(60);
    
    // create instance after window initialization
    Game game;
    
    while (WindowShouldClose() == false && currentScreen != GameScreen::EXIT)
    {
        BeginDrawing();
        ClearBackground(grey);
        switch (currentScreen)
        {
        case GameScreen::MENU:
            game.menu.Update();
            game.menu.Draw();
            break;

        case GameScreen::GAMEPLAY:
            UpdateMusicStream(game.music);
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

