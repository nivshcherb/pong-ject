/* -------------------------------------------------------------------------- *
 *  Libraries
 * -------------------------------------------------------------------------- */

#include <thread>
    using std::this_thread::sleep_for;
    using std::chrono::milliseconds;

#include <iostream>

#include "linux_kbd_device.hpp"

using namespace std;

/* -------------------------------------------------------------------------- *
 *  Main
 * -------------------------------------------------------------------------- */

int main()
{
    KbdInput *kbd = LinuxKbdDevice::Init();

    size_t pressed_count = 0;
    size_t released_count = 0;

    kbd->Update();
    while (!kbd->IsPressed(KEY_ESC))
    {
        bool isup = kbd->IsUp(KEY_LEFTCTRL);
        bool isdown = kbd->IsDown(KEY_LEFTCTRL);
        pressed_count += kbd->IsPressed(KEY_LEFTCTRL);
        released_count += kbd->IsReleased(KEY_LEFTCTRL);

        cout << "\033c";
        cout << "Press ESC to exit\n\n";
        cout << "Left Ctrl\n";
        cout << "Up:\t\t" << isup << "\n";
        cout << "Down:\t\t" << isdown << "\n";
        cout << "Pressed:\t" << pressed_count << "\n";
        cout << "Released:\t" << released_count << endl;

        kbd->Update();

        sleep_for(milliseconds(10));
    }

    return 0;
}
