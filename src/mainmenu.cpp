#include "mainmenu.hpp"
#include <string>

MainMenu::MainMenu(){
    LoadResources();
}

void MainMenu::Draw(){
    DrawBackground();
    DrawStartButton();
    CheckStartButtonClick(GetMousePosition());
}

bool MainMenu::ShouldStartGame(){
    return startGame;
}

void MainMenu::LoadResources(){
    background = LoadTexture("Graphics/background.png");
    startButtonTexture = LoadTexture("Graphics/startbutton.png");

    font = LoadFontEx("Fonts/SPACE.ttf", 64, 0, 0); // Load the font

    startButtonRect.x = (GetScreenWidth() - startButtonTexture.width) / 2.0f;
    startButtonRect.y = GetScreenHeight() / 2.0f;
    startButtonRect.width = static_cast<float>(startButtonTexture.width);
    startButtonRect.height = static_cast<float>(startButtonTexture.height);
}

void MainMenu::UnloadResources(){
    UnloadTexture(background);
    UnloadTexture(startButtonTexture);
}

void MainMenu::DrawBackground(){
    DrawTexture(background, 0, 0, WHITE);
}

void MainMenu::DrawStartButton(){
    float centerX = GetScreenWidth() / 2.0f - startButtonTexture.width / 2.0f;
    float centerY = GetScreenHeight() / 2.0f - startButtonTexture.height / 2.0f;

    DrawTexture(startButtonTexture, centerX, centerY, WHITE);
}

void MainMenu::CheckStartButtonClick(Vector2 mousePos){
    if (CheckCollisionPointRec(mousePos, startButtonRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        startGame = true;
    }
}

void MainMenu::DrawText(){
    const char* text = "SPACESHIP INVADERS";
    int textWidth = MeasureTextEx(font, text, 64, 2).x;
    Vector2 textPos = { GetScreenWidth() / 2.0f - textWidth / 2.0f, startButtonRect.y - 200 };
    DrawTextEx(font, text, textPos, 64, 2, WHITE);
}
