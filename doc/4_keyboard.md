# Keyboard

## KbdInput

Create **KbdInput** Interface.
*KbdInput* describes the functionality required to read the state of the keyboard and test keys. Again, Not linux specific.
Method      |   Description
:--         |   :--
Update      |   Read the status of the keyboard.
IsDown      |   Check if a given key is currently down.
IsUp        |   Check if a given key is currently up.
IsPressed   |   Check if a given key was just pressed.
IsReleased  |   Check if a given key was just released.

Make sure *KbdInput* is as portable as possible.

## LinuxKbdDevice
Create **LInuxKbdDevice** that implements *KbdInput*.
We will read the state of the keyboard by openning "-kbd" file found in **/dev/input/** and reading a bitarray using **ioctl**. Unfortunatly, this will require us to run as root.

The name of the "-kbd" file depends on your connected device. Moreover, you might find more than one "-kbd" file. Filtering out the currect file isn't imposible, but for now we will just open and read all of them.

Use **glop** to recieve an array of all "-kbd" files and save all the ones you are able to open. It is a good idea to add an exception if no keyboard is found.

Method      |   Description
:--         |   :--
Ctor        |   Open all "-kbd" files and store them.
Dtor        |   Close all the stored files.
Update      |   Read the keyboard state from the stored files.
IsDown      |   Check if the bit of a given key is unset.
IsUp        |   Check if the bit of a given key is set.
IsPressed   |   Check if the bit of a given key changed from unset to set.
IsReleased  |   Check if the bit of a given key changed from set to unset.

Use **linux/input.h** to get the keys' bit value.

## Testing

Test *LinuxKbdDevice* and make sure it works.
In a loop, check if a key is down or up and count the number of presses and releases.
Do not forget to run as root.

## Relevant Files

- [interface/kbd_input.hpp](../Pong/interface/kbd_input.hpp)
- [include/linux_kbd_device.hpp](../Pong/include/linux_kbd_device.hpp)
- [src/linux_kbd_device.hpp](../Pong/src/linux_kbd_device.cpp)