#include <raylib.h>
#include "game.hpp"
#include "mainmenu.hpp"
#include <string>
using namespace std;

string FormatWithLeadingZeros(int number, int width)
{
    string numberText = to_string(number);
    int leadingZeros = width - numberText.length();
    if (leadingZeros <= 0)
    {
        return numberText; // No leading zeros needed
    }
    return numberText = string(leadingZeros, '0') + numberText;
}

int main()
{

    // Color grey = {29, 29, 27, 255};
    Color black = {0, 0, 0, 255};
    // Color yellow = {243, 216, 63, 225};
    Color orange = {255, 179, 26, 255};

    int offset = 50;
    int WindowWidth = 950;
    int WindowHeight = 900;

    InitWindow(WindowWidth + offset, WindowHeight + 2 * offset, "C++ Space Ship");
    Font font = LoadFontEx("Fonts/monogram.ttf", 64, 0, 0);
    Texture2D LivesImage = LoadTexture("Graphics/lives.png");

    SetTargetFPS(60);
    InitAudioDevice();

    MainMenu mainmenu;
    Game game;

    Music menuMusic = LoadMusicStream("Sounds/menu.mp3");

    bool startGame = false;

    // Display the main menu
    while (!WindowShouldClose() && !startGame)
    {
        BeginDrawing();
        ClearBackground(black);

        UpdateMusicStream(menuMusic);
        PlayMusicStream(menuMusic);

        mainmenu.Draw();
        mainmenu.DrawText();

        if (mainmenu.ShouldStartGame())
        {
            startGame = true;
        }

        EndDrawing();
    }

    // Stop the menu music once the game starts
    StopMusicStream(menuMusic);
    UnloadMusicStream(menuMusic);

    while (WindowShouldClose() == false)
    {

        BeginDrawing();
        ClearBackground(black);

        UpdateMusicStream(game.music);
        if (game.IsMusicPlaying)
        {
            PlayMusicStream(game.music);
        }
        else
        {
            StopMusicStream(game.music);
        }

        game.HandleInput();
        game.Update();

        DrawRectangleRoundedLines({10, 10, 980, 980}, 0.18f, 20, 2, orange);
        DrawLineEx({25, 920}, {975, 920}, 3, orange);

        if (game.run)
        {
            DrawTextEx(font, "LEVEL 01", {770, 935}, 34, 2, orange);
        }
        else
        {
            DrawTextEx(font, "GAME OVER", {770, 935}, 34, 2, orange);
        }

        float x = 50.0;
        for (int i = 0; i < game.lives; i++)
        {
            DrawTextureV(LivesImage, {x, 925}, WHITE);
            x += 50;
        }

        DrawTextEx(font, "SCORE", {60, 35}, 34, 2, orange);
        string scoreText = FormatWithLeadingZeros(game.score, 5);
        DrawTextEx(font, scoreText.c_str(), {60, 60}, 34, 2, orange);

        DrawTextEx(font, "HIGH-SCORE", {750, 35}, 34, 2, orange);
        std::string highscoreText = FormatWithLeadingZeros(game.highscore, 5);
        DrawTextEx(font, highscoreText.c_str(), {835, 60}, 34, 2, orange);

        game.Draw();
        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();
}