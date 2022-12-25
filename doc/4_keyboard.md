# Keyboard

<img src="https://media.tenor.com/MmP6YbJNV2sAAAAC/wow-typing.gif" alt="keyboard"/>

Next, we will handle the *HandleInput()* step of the gameloop. While some game engines are more event driven, we will read the state of the keyboard every loop and using this information determine which keys are pressed.

## KbdInput

Again, the game and engine do not really care how we know what keys are presses, they only cares about the state of the keys.
Create *KbdInput* interface that will provide the game with the ability to test the state of any keyboard keys.

Method      |   Description
:--         |   :--
Update      |   Read the state of the keyboard.
IsDown      |   Check if a given key is currently down.
IsUp        |   Check if a given key is currently up.
IsPressed   |   Check if a given key was just pressed.
IsReleased  |   Check if a given key was just released.

## LinuxKbdDevice

For this game, we are going to try and read from the kbd files found in */dev/input/* directory using *ioctl*. Unfortunatly, this will require root privileges.

Inside */dev/input/* you might find more than one *-kbd* file. If you know which one to open, more power to you. For now, we'll just read all of them. You can use *glob*
to receive an array of files ending with *-kbd* and open each one of them.

Method      |   Description
:--         |   :--
Ctor        |   Open all "-kbd" files and store them.
Dtor        |   Close all the stored files.
Update      |   Read the keyboard state from the stored files.

The bit value for each key is found in *linux/input.h*.
Handle cases where you are unable to access any *-kbd* file.

## Testing

In a loop, check if a given key is pressed and count the number of presses and releases. Do not forget to run as root.

## Relevant Files

- [interface/kbd_input.hpp](../Pong/interface/kbd_input.hpp)
- [include/linux_kbd_device.hpp](../Pong/include/linux_kbd_device.hpp)
- [src/linux_kbd_device.hpp](../Pong/src/linux_kbd_device.cpp)

## Next chapter

[5. Game Engine](5_game_engine.md)