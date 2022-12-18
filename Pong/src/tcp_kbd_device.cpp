/* -------------------------------------------------------------------------- *
 *  Libraries
 * -------------------------------------------------------------------------- */

#include <stdexcept>
    using std::runtime_error;
    using std::out_of_range;

#include <cstring>
    using std::memset;

#include <linux/input.h>
    // KEY macros

#include "tcp_kbd_device.hpp"

/* -------------------------------------------------------------------------- *
 *  TcpKbdDevice Implementations
 * -------------------------------------------------------------------------- */

TcpKbdDevice::TcpKbdDevice(ConnectionReceiver<char[STATE_SIZE]> *src_) :
    m_src(src_),
    m_curr{ 0 },
    m_prev{ 0 }
{
    // Do nothing
}

void TcpKbdDevice::Update()
{
    char new_state[STATE_SIZE]{ 0 };

    if (!m_src->Receive(new_state)) return;

    // Save states
    for (size_t i = 0; i < STATE_SIZE; ++i)
    {
        m_prev[i] = m_curr[i];
        m_curr[i] = new_state[i];
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