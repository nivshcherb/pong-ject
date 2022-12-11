#ifndef __GAME_ENGINE_HPP__
#define __GAME_ENGINE_HPP__
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- *
 *  Libraries
 * -------------------------------------------------------------------------- */

#include "surface_window.hpp"

#include "game_logic.hpp"

/* -------------------------------------------------------------------------- *
 *  Classes
 * -------------------------------------------------------------------------- */

class GameEngine
{
public:
    explicit GameEngine();
    ~GameEngine();

    void Start();
    void End();

private:
    explicit GameEngine();

    // Members
    GameLogic *game;
    SurfaceWindow *window;
    bool is_running;
};

/* -------------------------------------------------------------------------- */
#endif /* __GAME_ENGINE_HPP__ */