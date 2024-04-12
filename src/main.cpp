#include <raylib.h>
#include "game.hpp"

int main()
{
    int windowWidth = 750;
    int windowHeight = 750;
    Color grey = {29, 29, 29, 255};
    SetTargetFPS(60);

    InitWindow(windowWidth, windowHeight, "C++ space invader");
    // create instance after window initialization
    Game game;
    
    while (WindowShouldClose() == false)
    {
        game.HandleInput();

        BeginDrawing();
        ClearBackground(grey);
        game.Draw();
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
