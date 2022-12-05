#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- *
 *  Libraries
 * -------------------------------------------------------------------------- */

#include <utility/utility.h>

#include "surface.hpp"

/* -------------------------------------------------------------------------- *
 *  Window Interface
 * -------------------------------------------------------------------------- */

class Window
{
    interface(Window);
    uncopyable(Window);

public:
    // Apply a given surface to the window at a given location.
    virtual void Apply(const Surface &surface, int x, int y) = 0;

    // Remove all applied surfaces from the window.
    virtual void Clean() = 0;

    // Output the window.
    virtual void Draw() = 0;

}; interface_req(Window);


/* -------------------------------------------------------------------------- */
#endif // __WINDOW_HPP__