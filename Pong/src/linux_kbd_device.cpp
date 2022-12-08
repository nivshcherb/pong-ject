/* -------------------------------------------------------------------------- *
 *  Libraries
 * -------------------------------------------------------------------------- */

#include <stdexcept>
    using std::runtime_error;
    using std::out_of_range;

#include <cstring>
    using std::memset;

#include <glob.h>
    // glob

#include <linux/input.h>
    // KEY macros

#include "linux_kbd_device.hpp"

/* -------------------------------------------------------------------------- *
 *  LinuxKbdDevice Implementations
 * -------------------------------------------------------------------------- */

LinuxKbdDevice::LinuxKbdDevice() :
    m_kbd(),
    m_prev{ 0 },
    m_curr{ 0 }
{
    // Get all "-kbd" files
    glob_t kbddev;
    glob("/dev/input/by-id/*-kbd", 0, nullptr, &kbddev);

    // For each keyboard
    for (size_t i = 0; i < kbddev.gl_pathc; ++i)
    {
        // Open file and push to keyboards vector
        FILE *device = fopen(kbddev.gl_pathv[i], "r");

        if (nullptr == device) continue;    // Skip if can't open

        m_kbd.push_back(device);
    }

    if (0 == m_kbd.size())  // If no keyboards found
    {
        throw(runtime_error("Unable to connect to keyboard..."));
    }
}

LinuxKbdDevice::~LinuxKbdDevice()
{
    for (auto iter = m_kbd.begin(); iter != m_kbd.end(); ++iter)
    {
        fclose(*iter);
    }
}

void LinuxKbdDevice::Update()
{
    // Save old state
    for (size_t i = 0; i < STATE_SIZE; ++i)
    {
        m_prev[i] = m_curr[i];
    }

    // Clean current state
    memset(m_curr, 0, STATE_SIZE);

    for (auto iter = m_kbd.begin(); iter != m_kbd.end(); ++iter)
    {
        // Read state
        char state[STATE_SIZE] = { 0 };
        ioctl(fileno(*iter), EVIOCGKEY(STATE_SIZE), &state);

        // Sum states
        for (size_t i = 0; i < STATE_SIZE; ++i)
        {
            m_curr[i] |= state[i];
        }
    }
}

bool LinuxKbdDevice::IsDown(int key_)
{
    return Test(m_curr, key_);
}

bool LinuxKbdDevice::IsUp(int key_)
{
    return !Test(m_curr, key_);
}

bool LinuxKbdDevice::IsPressed(int key_)
{
    // Previously up, currently down
    return !Test(m_prev, key_) && Test(m_curr, key_);
}

bool LinuxKbdDevice::IsReleased(int key_)
{
    // Previously down, currently up
    return Test(m_prev, key_) && !Test(m_curr, key_);
}

bool LinuxKbdDevice::Test(char *bitarr_, int key_)
{
    if ((key_ < 0) || (key_ > KEY_MAX))
    {
        throw(out_of_range("Invalid key..."));
    }

    size_t index = key_ / __CHAR_BIT__;
    size_t byte = key_ % __CHAR_BIT__;

    return bitarr_[index] & (1U << byte);
}

LinuxKbdDevice *LinuxKbdDevice::Init()
{
    static LinuxKbdDevice instance;

    return &instance;
}










