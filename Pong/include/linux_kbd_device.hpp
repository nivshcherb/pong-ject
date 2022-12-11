#ifndef __LINUX_KBD_DEVICE_HPP__
#define __LINUX_KBD_DEVICE_HPP__
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- *
 *  Libraries
 * -------------------------------------------------------------------------- */

#include <cstdio>
    using std::FILE;

#include <vector>
    using std::vector;

#include "kbd_input.hpp"

// KEY_MAX bits in bytes
constexpr size_t STATE_SIZE = (KEY_MAX + __CHAR_BIT__ - 1) / __CHAR_BIT__;

/* -------------------------------------------------------------------------- *
 *  Classes
 * -------------------------------------------------------------------------- */

class LinuxKbdDevice : public KbdInput
{
public:
    explicit LinuxKbdDevice();
    ~LinuxKbdDevice();

    // Uncopyable KbdInput

    // Override methods
    void Update() override;
    bool IsDown(int key_) override;  
    bool IsUp(int key_) override;
    bool IsPressed(int key_) override;
    bool IsReleased(int key_) override;

private:
    static bool Test(char *bitarr_, int index_);

    // Members
    vector<FILE*> m_kbd;
    char m_prev[STATE_SIZE];
    char m_curr[STATE_SIZE];
};

/* -------------------------------------------------------------------------- */
#endif /* __LINUX_KBD_DEVICE_HPP__ */