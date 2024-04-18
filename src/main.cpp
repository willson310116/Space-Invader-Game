#include <raylib.h>
#include "game.hpp"
#include "menu.hpp"

enum class GameScreen { MENU = 0, GAMEPLAY};

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
    Menu menu;
    GameScreen currentScreen = GameScreen::MENU;
    bool exitGame = false;

    while (WindowShouldClose() == false && !exitGame)
    {
        BeginDrawing();
        ClearBackground(grey);
        switch (currentScreen)
        {
        case GameScreen::MENU:
            menu.Update();
            menu.Draw();
            if (menu.startButton.clicked)
                currentScreen = GameScreen::GAMEPLAY;
            else if (menu.exitButton.clicked)
                exitGame = true;
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

