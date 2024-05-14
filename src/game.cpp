#include "game.hpp"
#include <iostream>
#include <fstream>
using namespace std;

Game::Game(){
    InitGame();
    music = LoadMusicStream("Sounds/music.mp3");
    PlayMusicStream(music);
    explode = LoadSound("Sounds/explode.mp3");
    gameover = LoadSound("Sounds/gameover.mp3");
}

Game::~Game(){
    Alien::UnloadImages();
    UnloadSound(explode);
    UnloadMusicStream(music);
    UnloadSound(gameover);
}

void Game::Update(){
    if(run){
        for(auto& laser: spaceship.lasers){
            laser.Update();
        }

        MoveAlien();

        AlienShootLaser();

        for(auto& laser: alienLasers){
            laser.Update();
        }

        DeleteInactiveLaser();

        CheckForCollision();
    }
}

void Game::Draw(){
    spaceship.Draw();

    for(auto& laser: spaceship.lasers){
        laser.Draw();
    }

    /*for(auto& obstacle: obstacles){
        obstacle.Draw();
    }*/

    for(auto& alien: aliens){
        alien.Draw();
    }

    for(auto& laser: alienLasers){
        laser.Draw();
    }
}

void Game::HandleInput(){
    if(run){
        if(IsKeyDown(KEY_LEFT)){
            spaceship.MoveLeft();
        }else if(IsKeyDown(KEY_RIGHT)){
            spaceship.MoveRight();
        }else if(IsKeyDown(KEY_SPACE)){
            spaceship.FireLaser();
        }else if(IsKeyDown(KEY_UP)){
            spaceship.MoveUp();
        }else if(IsKeyDown(KEY_DOWN)){
            spaceship.MoveDown();
        }
    }else{
        if(IsKeyDown(KEY_ENTER)){
            Reset();
            InitGame();
        }
    }
}

void Game::DeleteInactiveLaser(){
    for(auto it = spaceship.lasers.begin(); it != spaceship.lasers.end();){
        if(!it -> active){
            it = spaceship.lasers.erase(it);
        }else{
            ++it;
        }
    }

    for(auto it = alienLasers.begin(); it != alienLasers.end();){
        if(!it -> active){
            it = alienLasers.erase(it);
        }else{
            ++it;
        }
    }
}

vector<Obstacle> Game::CreateObstacle(){
    int obstacleWidth = Obstacle::grid[0].size() * 3;
    float distance = (GetScreenWidth() - (4 * obstacleWidth))/5;

    for(int i = 0; i < 4; i++){
        float offsetX = (i + 1) * distance + i * obstacleWidth;
        obstacles.push_back(Obstacle({offsetX, float(GetScreenHeight() - 100)}));
    }
    return obstacles;
}

void Game::InitGame(){
    obstacles = CreateObstacle();
    aliens = CreateAliens();
    aliensDirection = 1;
    timeLastAlienFired = 0.0;
    lives = 3;
    run = true;
    score = 0;
    highscore = loadHighscoreFromFile();
    
    IsMusicPlaying = true;
}

void Game::checkForHighscore(){
    if(score > highscore){
        highscore = score;
        saveHighscoreToFile(highscore);
    }
}

void Game::saveHighscoreToFile(int highscore){
    ofstream highscoreFile("highscore.txt");
    if(highscoreFile.is_open()){
        highscoreFile << highscore;
        highscoreFile.close();
    }else{
        cerr << "Failed To Save highscore to File" << endl;
    }
}

int Game::loadHighscoreFromFile(){
    int loadedHighscore = 0;
    ifstream highscoreFile("highscore.txt");
    if(highscoreFile.is_open()){
        highscoreFile >> loadedHighscore;
        highscoreFile.close();
    }else{
        cerr << "Failed to load highscore from file" << endl;
    }
    return loadedHighscore;
}

void Game::Reset(){
    spaceship.Reset();
    aliens.clear();
    alienLasers.clear();
    obstacles.clear();
}

vector<Alien> Game::CreateAliens()
{
    std::vector<Alien> aliens;
    for(int row = 0; row < 5; row++) {
        for(int column = 0; column < 11; column++) {

            int alienType;
            if(row == 0) {
                alienType = 3;
            } else if (row == 1 || row == 2) {
                alienType = 2;
            } else {
                alienType = 1;
            }

            float x = 185 + column * 55;
            float y = 110 + row * 55;
            aliens.push_back(Alien(alienType, {x, y}));
        }
    }
    return aliens;
}

void Game::MoveAlien(){
    for(auto& alien: aliens){
        if(alien.position.x + alien.alienImages[alien.type - 1].width > GetScreenWidth() - 25){
            aliensDirection = -1;
            MoveDownAliens(4);
        }
        if(alien.position.x < 25){
            aliensDirection = 1;
            MoveDownAliens(4);
        }
        alien.Update(aliensDirection);
    }
}

void Game::MoveDownAliens(int distance){
    for(auto& alien: aliens){
        alien.position.y += distance;
    }
}

void Game::AlienShootLaser(){

    double CurrentTime = GetTime();
    if(CurrentTime - timeLastAlienFired >= alienLaserShootInterval && !aliens.empty()){
        int randomIndex = GetRandomValue(0, aliens.size() - 1);
        Alien& alien = aliens[randomIndex];
        alienLasers.push_back(Laser({alien.position.x + alien.alienImages[alien.type - 1].width/2, alien.position.y + alien.alienImages[alien.type - 1].height}, 6));
        timeLastAlienFired = GetTime();
    }
}

void Game::CheckForCollision(){
    for(auto& laser: spaceship.lasers){
        auto it = aliens.begin();
        while(it != aliens.end()){
            if(CheckCollisionRecs(it -> getRect(), laser.getRect())){

                if(it -> type == 1){
                    score += 100;
                }else if(it -> type == 2){
                    score += 200;
                }else if(it -> type == 3){
                    score += 300;
                }

                checkForHighscore();

                it = aliens.erase(it);
                laser.active = false;
                PlaySound(explode);
            }else{
                ++it;
            }
        }

        /*for(auto&obstacle: obstacles){
            auto it = obstacle.blocks.begin();
            while(it != obstacle.blocks.end()){
                if(CheckCollisionRecs(it -> getRect(), laser.getRect())){
                    it = obstacle.blocks.erase(it);
                    laser.active = false;
                }else{
                    ++it;
                }
            }
        }*/
    }

    for(auto& laser: alienLasers){
        if(CheckCollisionRecs(laser.getRect(), spaceship.getRect())){
            Sound ouch = LoadSound("Sounds/ouch.mp3");
            laser.active = false;
            lives --;
            PlaySound(ouch);
            if(lives == 0){
                GameOver();
                PlaySound(gameover);
            }
        }
    }

    for(auto& alien: aliens) {
    for(auto& obstacle: obstacles) {
        auto it = obstacle.blocks.begin();
        while(it != obstacle.blocks.end()) {
            if(CheckCollisionRecs(it -> getRect(), alien.getRect())) {
                it = obstacle.blocks.erase(it);
            } else {
                it ++;
            }
        }
    }

    if(CheckCollisionRecs(alien.getRect(), spaceship.getRect())) {
            GameOver();
        }
    }
}

void Game::GameOver()
{
    run = false;
    IsMusicPlaying = false;
    StopMusicStream(music);
}
