#pragma once
#include <raylib.h>

class Alien{
    public:
        Alien(int type, Vector2 position);
        void Update(int direction);
        void Draw();
        int GetType();
        static Texture2D alienImages[3];
        static void UnloadImages();
        Rectangle getRect();
        int type;
        Vector2 position;

    private:
};