#include <raylib.h>
#include <string>
#include "game.hpp"

std::string FormatWithLeadingZeros(int number, int width)
{
    std::string numberText = std::to_string(number);
    int leadingZeros = width - numberText.size();
    return std::string(leadingZeros, '0') + numberText;
}

int main()
{
    int windowWidth = 750;
    int windowHeight = 700;
    int offset = 50;
    Color grey = {29, 29, 29, 255};
    Color red = {225, 10, 10, 255};
    Color yellow = {243, 216, 63, 255};

    InitWindow(windowWidth + offset, windowHeight + 2 * offset, "C++ space invader");
    InitAudioDevice();
    SetTargetFPS(60);
    Font font = LoadFontEx("../asset/font/monogram.ttf", 64, 0, 0);
    Texture2D spaceshipImage = LoadTexture("../asset/graphics/spaceship.png");
    // create instance after window initialization
    Game game;
    
    while (WindowShouldClose() == false)
    {
        UpdateMusicStream(game.music);
        game.HandleInput();
        game.Update();
        
        BeginDrawing();
        ClearBackground(grey);
        DrawRectangleRoundedLines({10, 10, 780, 780}, 0.18f, 20, 2, yellow);
        DrawLineEx({25, 730}, {775, 730}, 3, yellow);
        game.Draw();

        if (game.run)
            DrawTextEx(font, "LEVEL 01", {570, 740}, 34, 2, yellow);
        else
        {
            DrawTextEx(font, "GAME OVER", {570, 740}, 34, 2, yellow);
            DrawTextEx(font, "PRESS ENTER TO RESTART", {180, 50}, 36, 2, red);
            // DrawTextEx(font, "PRESS ESC TO LEAVE", {200, 80}, 36, 2, red);
        }
            
        
        DrawTextEx(font, "LIVES", {70, 740}, 34, 2, yellow);
        float x = 180.0;
        for (int i = 0; i < game.lives; i++)
        {
            DrawTextureV(spaceshipImage, {x, 740}, WHITE);
            x += 50;
        }
        
        // std::string score = "SCORE: " + std::to_string(game.score);
        // DrawTextEx(font, score.c_str(), {50, 15}, 36, 2, yellow);
        DrawTextEx(font, "SCORE", {50, 15}, 36, 2, yellow);
        std::string scoreText = FormatWithLeadingZeros(game.score, 5);
        DrawTextEx(font, scoreText.c_str(), {50, 40}, 36, 2, yellow);

        DrawTextEx(font, "HIGH-SCORE", {570, 15}, 36, 2, yellow);
        std::string highScoreText = FormatWithLeadingZeros(game.highScore, 5);
        DrawTextEx(font, highScoreText.c_str(), {665, 40}, 36, 2, yellow);
        

        EndDrawing();
    }

    CloseWindow();
    CloseAudioDevice();
    return 0;
}
