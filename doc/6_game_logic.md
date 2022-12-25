# Game Logic

<img src="https://media.tenor.com/ptYJsG8-K4MAAAAC/cats-ping-pong.gif" alt="game_logic"/>

Make sure you understand the rules of Pong, since this is where the actual game will be writen. Consider what information you might need to store and the steps in which the logic itself is executed. Write a breakdown of each entity's behavior.

Create *Pong* class that implements *GameLogic*. And follow the next steps to implement each method.

## Initialize

Give some default value to everything. Create and store an instance of *KbdInput*
that will be later used in *HandleInput()*. Decide which keyboard class to use based on the defined macro in *config.h*, *LOCAL_KEYBOARD_CLASS*.

You can also create *KbdInput* inside *Ctor()* and use *Initialize()* to also restart the game.

## HandleInput

Decide on keys for the 2 players, quit, pause / resume and restart. It's best to define these as constant / macros so changing them later will be easier.

If you would like, you can also read these keys from a external file.

First, update the state of your keyboard. Then, check each key and handle accordingly.
For now, only handle the players' movement. 

Since the time it takes each loop to complete is not constant, consider moving the players relative to a delta of the time since the previus call. This delta will also be used in *Update()*

You can also save a variable telling *Update()* what input was provided to the game and change the players' position in *Update()*.

## Update

Write all the rules of Pong in this method:
1. Both players can not leave the game's boundaries.
2. The ball moves in a given direction until it collides with other entities.
3. Colliding with the upper and lower boundaries will cause the ball to bounce.
4. Collising with either player will cause the ball to bounce.
5. Collising with the left and right boundaries will give a point to a player.
6. When one of the player reaches a given value of points, they win.

## Draw

Start the function by calling *Clean()* and end it with *Draw()*.
Between these two, apply a player *Surface* in the location of each player and a ball *Surface* in the location of the ball.

Later on, you can add a verical line in the middle and score for each player.

## IsRunning

Decide on some quit condition (Esc key, for example).

## State Machine

We do not want the game to run all the time. In order to allow pausing we need to change *HandleInput()* and *Update()* to first check if the game is currently running or not.

A better solution is handling the game with a state machine. Create a list of possible states for the game (Running, Paused, etc.) and store the current state as a variable in *Pong* class.

Change *HandleInput()* and *Update()* to first check the current state of the game and then do their thing. For each state, consider the possible inputs and updates needed. Drawing out the state machine on a piece of paper will help a lot.

Also give different behavior to *Draw()* based on the current state. Maybe write a nice "player x wins* message of *paused*. It's up to you.

## Relevant Files

- [interface/pong.hpp](../Pong/interface/pong.hpp)
- [src/pong.cpp](../Pong/src/pong.cpp)

## Next chapter

[7. TCP](7_tcp.md)