# Game Engine

<img src="https://media.tenor.com/CCiP_z843YUAAAAC/kronk-its-all-coming-together.gif" alt="game_engine"/>

## GameLogic

The last interface to provide to the game engine is *GameLogic*, which includes
all the methods for the actual game we would like to run.

The complete gameloop will loop like this:

1. Initialization
2. In a loop, while the game is running
    1. handle any input
    2. update the game base on some logic / input
    3. output the game using some graphics class
3. Clean up

We want *GameLogic* to include all 5 methods.

Method  |   Description
:--     |   :--
Initialize      |   Allocate and set all resources needed by the game
HandleInput     |   Update all input devices and handle the possible inputs
Update          |   Update the game
Draw            |   Set all surfaces to be drawn by the window object
Deinitialize    |   Release all allocated resources
IsRunning       |   Used by the game engine to check if the game is running

## GameEngine

It's time to connect everything we have so far and set the game engine up.
Create *GameEngine* class that will include a single method, *Start()*, that
runs a given game logic and allows it to apply *Surface*s and draw them.

*GameEngine* will know which *GameLogic* and *SurfaceWindow* to initialize by taking
the values defined in *config.h*, *GAME_CLASS* and *GRAPHIC_CLASS*.

Add *main()* to the implementation of *GameEngine* that will create an instance of *GameEngine* and call it's *Start()* method. Make sure *GameEngine* closes gracefully even when an exception is thrown.

## Relevant Files

- [interface/game_logic.hpp](../Pong/interface/game_logic.hpp)
- [include/game_engine.hpp](../Pong/include/game_engine.hpp)
- [src/game_engine.cpp](../Pong/src/game_engine.cpp)

## Next chapter

[6. Game Logic](6_game_logic.md)