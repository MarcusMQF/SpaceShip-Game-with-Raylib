#pragma once
#include "Spaceship.hpp"
#include "obstacle.hpp"
#include "alien.hpp"
#include <raylib.h>
using namespace std;

class Game{
    public:
        Game();
        ~Game();
    
        void Draw();
        void Update();
        void HandleInput();
        bool run;
        int lives;
        int score;
        int highscore;
        Music music;
        bool IsMusicPlaying = false;

    private:
        Spaceship spaceship;
        void DeleteInactiveLaser();
        vector<Obstacle> CreateObstacle();
        vector<Obstacle> obstacles;
        vector<Alien> aliens;
        void MoveAlien();
        void MoveDownAliens(int distance);
        void AlienShootLaser();
        void CheckForCollision();
        void GameOver();
        void Reset();
        void InitGame();
        void checkForHighscore();
        void saveHighscoreToFile(int highscore);
        int loadHighscoreFromFile();
        vector<Alien> CreateAliens();
        int aliensDirection;
        vector<Laser> alienLasers;
        constexpr static float alienLaserShootInterval = 0.35;
        float timeLastAlienFired;
        Sound explode;
        Sound gameover;
};