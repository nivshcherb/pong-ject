#ifndef __KBD_INPUT_INTERFACE_HPP__
#define __KBD_INPUT_INTERFACE_HPP__
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- *
 *  Libraries
 * -------------------------------------------------------------------------- */

#include <linux/input.h>

/* -------------------------------------------------------------------------- *
 *  Interface
 * -------------------------------------------------------------------------- */

class KbdInput  // Interface
{
public:
    explicit KbdInput() = default;
    virtual ~KbdInput() = 0;

    // Uncopyable
    KbdInput(const KbdInput&) = delete;
    KbdInput& operator=(const KbdInput&) = delete;

    // Read the status of the keyboard
    virtual void Update() = 0;

    // Check if a given key is currently down
    virtual bool IsDown(int key_) = 0;  

    // Check if a given key is currently up
    virtual bool IsUp(int key_) = 0;

    // Check if a given key was just pressed
    virtual bool IsPressed(int key_) = 0;

    // Check if a given key was just released
    virtual bool IsReleased(int key_) = 0;
};

inline KbdInput::~KbdInput() {}

/* -------------------------------------------------------------------------- */
#endif /* __KBD_INPUT_INTERFACE_HPP__ */