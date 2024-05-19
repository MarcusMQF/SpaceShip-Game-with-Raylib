#pragma once

#include "raylib.h"

class MainMenu {
public:
    MainMenu();
    void Draw();
    bool ShouldStartGame();
    void DrawText();

private:
    bool startGame = false;
    Texture2D background;
    Rectangle startButtonRect;
    Texture2D startButtonTexture;
    Font font;

    void LoadResources();
    void UnloadResources();
    void DrawBackground();
    void DrawStartButton();
    void CheckStartButtonClick(Vector2 mousePos);
};