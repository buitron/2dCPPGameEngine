#include <iostream>
#include <map>

#include "../lib/glm/glm.hpp"
#include "./AssetManager.h"
#include "./Constants.h"
#include "./Map.h"
#include "./Game.h"
#include "./Components/ColliderComponent.h"
#include "./Components/ProjectileEmitterComponent.h"
#include "./Components/KeyboardControlComponent.h"
#include "./Components/SpriteComponent.h"
#include "./Components/TextLabelComponent.h"
#include "./Components/TransformComponent.h"

EntityManager manager;
AssetManager* Game::mp_assetManager = new AssetManager(&manager);
SDL_Renderer* Game::mp_renderer = 0;
SDL_Event Game::m_event;
SDL_Rect Game::m_camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
Entity* mainPlayer = 0;
Map* Game::mp_map;
std::map<char, int> playerMapping; // Map player lua to use in Update() function for projectile details

Game::Game()
{
    m_ticksLastFrame = 0;
    this->m_isRunning = false;
}

Game::~Game() {}

bool Game::IsRunning() const
{
    return this->m_isRunning;
}

void Game::Initialize(int width, int height)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << "Error initializing SDL." << std::endl;
        return;
    }

    if (TTF_Init() != 0)
    {
        std::cerr << "Error intializing SDL TTF." << std::endl;
        return;
    }

    mp_window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_BORDERLESS);
    if (!mp_window)
    {
        std::cerr << "Error creating SDL window." << std::endl;
        return;
    }

    mp_renderer = SDL_CreateRenderer(mp_window, -1, 0);
    if (!mp_renderer)
    {
        std::cerr << "Error creating the renderer." << std::endl;
        return;
    }

    LoadLevel(1);
    m_isRunning = true;
    return;
}


void Game::LoadLevel(int levelNumber)
{
    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::os, sol::lib::math);

    std::string levelName = "Level" + std::to_string(levelNumber);
    lua.script_file("./assets/scripts/" + levelName + ".lua");

    /*************************************/
    /* LOADS ASSETS FROM LUA CONFIG FILE */
    /*************************************/
    sol::table levelData = lua[levelName];
    sol::table levelAssets = levelData["assets"];

    unsigned int assetIndex = 0;
    while (true)
    {
        sol::optional<sol::table> existsAssetIndexNode = levelAssets[assetIndex];
        if (existsAssetIndexNode == sol::nullopt)
        {
            break;
        }
        else
        {
            sol::table asset = levelAssets[assetIndex];
            std::string assetType = asset["type"];

            if (assetType.compare("texture") == 0)
            {
                std::string assetId = asset["id"];
                std::string assetFile = asset["file"];
                mp_assetManager->AddTexture(assetId, assetFile.c_str());
            }
            if (assetType.compare("font") == 0)
            {
                std::string assetId = asset["id"];
                std::string assetFile = asset["file"];
                int fontSize = asset["fontSize"];
                mp_assetManager->AddFont(assetId, assetFile.c_str(), fontSize);
            }
        }
        ++assetIndex;
    }
    
    /**********************************/
    /* LOADS MAP FROM LUA CONFIG FILE */
    /**********************************/
    sol::table levelMap = levelData["map"];
    std::string mapTextureId = levelMap["textureAssetId"];
    std::string mapFile = levelMap["file"];

    mp_map = new Map(mapTextureId, static_cast<int>(levelMap["scale"]), static_cast<int>(levelMap["tileSize"]));
    mp_map->LoadMap(mapFile, static_cast<int>(levelMap["mapSizeX"]), static_cast<int>(levelMap["mapSizeY"]));

    /***************************************/
    /* LOADS ENTITIES FROM LUA CONFIG FILE */
    /***************************************/
    sol::table levelEntities = levelData["entities"];

    unsigned int entityIndex = 0;
    while (true)
    {
        sol::optional<sol::table> existsEntityIndexNode = levelEntities[entityIndex];
        if (existsEntityIndexNode == sol::nullopt)
        {
            break;
        }
        else
        {
            sol::table entity = levelEntities[entityIndex];
            std::string entityName = entity["name"];
            LayerType entityLayerType = static_cast<LayerType>(static_cast<int>(entity["layer"]));
            
            // Add a new entity
            Entity& newEntity(manager.AddEntity(entityName, entityLayerType));
            
            // Add a transform component
            sol::table entityComponents = entity["components"];
            sol::optional<sol::table> existsTransformComponent = entityComponents["transform"];
            if (existsTransformComponent != sol::nullopt)
            {
                sol::table transformComponent = entityComponents["transform"];
                int positionX = static_cast<int>(transformComponent["position"]["x"]);
                int positionY = static_cast<int>(transformComponent["position"]["y"]);
                int velocityX = static_cast<int>(transformComponent["velocity"]["x"]);
                int velocityY = static_cast<int>(transformComponent["velocity"]["y"]);
                int facingDeg = static_cast<int>(transformComponent["facingDegree"]);
                int width = static_cast<int>(transformComponent["width"]);
                int height = static_cast<int>(transformComponent["height"]);
                int scale = static_cast<int>(transformComponent["scale"]);
                newEntity.AddComponent<TransformComponent>(positionX, positionY, velocityX, velocityY, facingDeg, width, height, scale);
            }
            
            // Add a sprite component
            sol::optional<sol::table> existsSpriteComponent = entityComponents["sprite"];
            if (existsSpriteComponent != sol::nullopt)
            {
                sol::table spriteComponent = entityComponents["sprite"];
                std::string spriteId = spriteComponent["textureAssetId"];
                if (static_cast<bool>(spriteComponent["animated"]))
                {
                    int frameCount = static_cast<int>(spriteComponent["frameCount"]);
                    int animationSpeed = static_cast<int>(spriteComponent["animationSpeed"]);
                    bool hasDirection = static_cast<bool>(spriteComponent["hasDirection"]);
                    bool fixed = static_cast<bool>(spriteComponent["fixed"]);          
                    newEntity.AddComponent<SpriteComponent>(spriteId,  frameCount, animationSpeed, hasDirection , fixed);
                }
                else
                {
                    newEntity.AddComponent<SpriteComponent>(spriteId);
                }
            }

            // Add collider component
            sol::optional<sol::table> existsColliderComponent = entityComponents["collider"];
            if (existsColliderComponent != sol::nullopt)
            {
                sol::table colliderComponent = entityComponents["collider"];
                int positionX = static_cast<int>(entityComponents["transform"]["position"]["x"]);
                int positionY = static_cast<int>(entityComponents["transform"]["position"]["y"]);
                int width = static_cast<int>(entityComponents["transform"]["width"]);
                int height = static_cast<int>(entityComponents["transform"]["height"]);
                newEntity.AddComponent<ColliderComponent>(colliderComponent["tag"], positionX, positionY, width, height);
            }

            // Add projectile emitter component
            sol::optional<sol::table> existsProjectileEmitter = entityComponents["projectileEmitter"];
            if (existsProjectileEmitter != sol::nullopt)
            {
                
                int parentEntityXPos = static_cast<int>(entityComponents["transform"]["position"]["x"]);
                int parentEntityYPos = static_cast<int>(entityComponents["transform"]["position"]["y"]);
                int parentEntityWidth = static_cast<int>(entityComponents["transform"]["width"]);
                int parentEntityHeight = static_cast<int>(entityComponents["transform"]["height"]);

                sol::table projectileComponent = entityComponents["projectileEmitter"];
                std::string textureAssetId = projectileComponent["textureAssetId"];

                int speed = projectileComponent["speed"];
                int range = projectileComponent["range"];
                int width = projectileComponent["width"];
                int height = projectileComponent["height"];
                bool shouldLoop = projectileComponent["shouldLoop"];

                if (entityName.compare("player") != 0)
                {
                    Entity& projectileEntity(manager.AddEntity("projectile", PROJECTILE_LAYER));
                    projectileEntity.AddComponent<TransformComponent>(parentEntityXPos + (parentEntityWidth/2), parentEntityYPos + (parentEntityHeight/2), 0, 0, width, height, 1);
                    projectileEntity.AddComponent<SpriteComponent>(textureAssetId);
                    projectileEntity.AddComponent<ProjectileEmitterComponent>(newEntity, speed, range, shouldLoop);
                    projectileEntity.AddComponent<ColliderComponent>("projectile", parentEntityXPos, parentEntityYPos, width, height);
                }
            }

            // Add input control component
            sol::optional<sol::table> existsInputSelection = entityComponents["input"];
            if (existsInputSelection != sol::nullopt)
            {
                sol::table inputSelection = entityComponents["input"];
                sol::optional<sol::table> existsKeyboardComponent = inputSelection["keyboard"];
                if (existsKeyboardComponent != sol::nullopt)
                {
                    sol::table keyboardComponent = inputSelection["keyboard"];
                    std::string up = keyboardComponent["up"];
                    std::string right = keyboardComponent["right"];
                    std::string down = keyboardComponent["down"];
                    std::string left = keyboardComponent["left"];
                    std::string shoot = keyboardComponent["shoot"];
                    newEntity.AddComponent<KeyboardControlComponent>(up, right, down, left, shoot);
                }
            }

            if (newEntity.m_name.compare("player") == 0)
            {
               mainPlayer = &newEntity;
            }
        }
    ++entityIndex;
    }
}

void Game::ProcessInput()
{
    SDL_PollEvent(&m_event);

    switch (m_event.type)
    {
        case SDL_QUIT: m_isRunning = false; break;
        case SDL_KEYDOWN: if (m_event.key.keysym.sym == SDLK_ESCAPE) m_isRunning = false; break;
        default: break;
    }
}

void Game::Update()
{
    // Wait until ~16.6ms has ellapsed since the last frame
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), m_ticksLastFrame + FRAME_TARGET_TIME));

    // Delta time is the difference in ticks from last frame converted to seconds
    float deltaTime = (SDL_GetTicks() - m_ticksLastFrame) / 1000.f;

    // Clamp deltaTime to a maximum value
    deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;

    // Sets the new ticks for the current frame to be used in the next pass
    m_ticksLastFrame = SDL_GetTicks();

    manager.Update(deltaTime);

    // If shooting create new entities to shoot
    if (mainPlayer->m_bang)
    {
        TransformComponent* mainTransform = mainPlayer->GetComponent<TransformComponent>();
        int mainWidth = mainTransform->m_width;
        int mainHeight = mainTransform->m_height;

        Entity& projectileEntity(manager.AddEntity("projectile", PROJECTILE_LAYER));
        projectileEntity.AddComponent<TransformComponent>(mainTransform->m_position.x + (mainWidth/2), mainTransform->m_position.y + (mainHeight/2), 0, 0, 4, 4, 1);
        projectileEntity.AddComponent<SpriteComponent>("friendly-projectile");
        projectileEntity.AddComponent<ProjectileEmitterComponent>(*mainPlayer, 120, 220, false);
        projectileEntity.AddComponent<ColliderComponent>("friendly-projectile", mainTransform->m_position.x, mainTransform->m_position.y, mainWidth, mainHeight);
    }

    HandleCameraMovement();
    CheckCollisions();
}

void Game::Render()
{
    SDL_SetRenderDrawColor(mp_renderer, 21, 21, 21, 255);
    SDL_RenderClear(mp_renderer);
    
    if (manager.HasNoEntities())
    {
        return;
    }
    
    manager.Render();
     
    SDL_RenderPresent(mp_renderer);
}

void Game::HandleCameraMovement()
{
    if (mainPlayer)
    {
        TransformComponent* mainPlayerTransform = mainPlayer->GetComponent<TransformComponent>();
        m_camera.x = mainPlayerTransform->m_position.x - (WINDOW_WIDTH/2);
        m_camera.y = mainPlayerTransform->m_position.y - (WINDOW_HEIGHT/2);

        // Clamping the values of the camera if it moves outside the tile mapping
        m_camera.x = m_camera.x < 0 ? 0 : m_camera.x;
        m_camera.y = m_camera.y < 0 ? 0 : m_camera.y;
        m_camera.x = m_camera.x > m_camera.w ? m_camera.w : m_camera.x;
        m_camera.y = m_camera.y > m_camera.h ? m_camera.h : m_camera.y;
    }
}

void Game::CheckCollisions()
{
    CollisionType collisionType = manager.CheckCollisions();
    if (collisionType == PLAYER_ENEMY_COLLISION) ProcessGameOver();
    if (collisionType == PLAYER_LEVEL_COMPLETE_COLLISION) ProcessNextLevel(1);
    if (collisionType == PLAYER_PROJECTILE_COLLISION) ProcessGameOver();
}

void Game::ProcessNextLevel(int levelNumber)
{
    std::cout << "Next Level" << std::endl;
    m_isRunning = false;
}

void Game::ProcessGameOver()
{
    std::cout << "Game Over" << std::endl;
    m_isRunning = false;
}


void Game::Destroy()
{
    SDL_DestroyRenderer(mp_renderer);
    SDL_DestroyWindow(mp_window);
    SDL_Quit();
}
