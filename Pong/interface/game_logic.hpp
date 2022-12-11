#ifndef __GAME_LOGIC_INTERFACE_HPP__
#define __GAME_LOGIC_INTERFACE_HPP__
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- *
 *  Libraries
 * -------------------------------------------------------------------------- */

#include "surface_window.hpp"

/* -------------------------------------------------------------------------- *
 *  Interface
 * -------------------------------------------------------------------------- */

class GameLogic // Interface
{
public:
    explicit GameLogic() = default;
    virtual ~GameLogic() = 0;

    //Uncopyable
    GameLogic(const GameLogic&) = delete;
    GameLogic& operator=(const GameLogic&) = delete;

    // Allocate all resources needed by the game
    virtual void Initialize() = 0;

    // Read and check all events given by the input device
    virtual void HandleInput() = 0;

    // Update the game information
    virtual void Update() = 0;

    // Set objects to be drawn on window
    virtual void Draw(SurfaceWindow *window_) = 0;

    // Release all allocated resources
    virtual void Deinitialize() = 0;
};

inline GameLogic::~GameLogic() {}

/* -------------------------------------------------------------------------- */
#endif /* __GAME_LOGIC_INTERFACE_HPP__ */