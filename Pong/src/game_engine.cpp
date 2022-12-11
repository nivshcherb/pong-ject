/* -------------------------------------------------------------------------- *
 *  Libraries
 * -------------------------------------------------------------------------- */

#include "game_engine.hpp"

#include "config.h"

/* -------------------------------------------------------------------------- *
 *  GameEngine Implementations
 * -------------------------------------------------------------------------- */

GameEngine::GameEngine() :
    game(nullptr),
    window(nullptr),
    is_running(false)
{
    // Do nothing
}

GameEngine::~GameEngine()
{
    // Do nothing
}

void GameEngine::Start()
{
    // Initialize
    window = new GRAPHIC_CLASS(CONFIG_NAME, CONFIG_WIDTH, CONFIG_HEIGHT);
    // game = new GAME_CLASS;
    game->Initialize();

    is_running = true;
    while (is_running)
    {
        game->HandleInput();

        game->Update();

        game->Draw();
    }

    // Deinitialize
    game->Deinitialize();
    //delete game; game = nullptr;
    delete window; window = nullptr;
}

void GameEngine::End()
{
    is_running = false;
}
