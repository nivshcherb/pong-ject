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
    window(nullptr)
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
    game = new GAME_CLASS;
    game->Initialize();

    while (game->IsRunning())
    {
        game->HandleInput();

        game->Update();

        game->Draw(window);
    }

    // Deinitialize
    game->Deinitialize();
    delete game; game = nullptr;
    delete window; window = nullptr;
}

/* -------------------------------------------------------------------------- *
 *  Main function
 * -------------------------------------------------------------------------- */

int main()
{
    try
    {
        GameEngine engine;
        engine.Start();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}
