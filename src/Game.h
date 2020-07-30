#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../lib/lua/sol.hpp"
#include "./Entity.h"
#include "./Component.h"
#include "./EntityManager.h"
#include "./Map.h"

class AssetManager;

class Game
{
    public:
        static SDL_Renderer* mp_renderer;
        static AssetManager* mp_assetManager;
        static SDL_Event m_event;
        static SDL_Rect m_camera;
        static Map* mp_map; 

        Game();
        ~Game();
        bool IsRunning() const;
        void LoadLevel(int levelNumber);
        void Initialize(int width, int height);
        void HandleCameraMovement();
        void ProcessNextLevel(int levelNumber);
        void ProcessGameOver();
        void CheckCollisions();
        void ProcessInput();
        void Update();
        void Render();
        void Destroy();
        sol::table GetLuaTable(int levelNumber);
    private:
        SDL_Window* mp_window;
        bool m_isRunning;
        int  m_ticksLastFrame;
};

#endif
