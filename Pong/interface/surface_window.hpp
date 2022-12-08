#ifndef __SURFACE_WINDOW_INTERFACE_HPP__
#define __SURFACE_WINDOW_INTERFACE_HPP__
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- *
 *  Libraries
 * -------------------------------------------------------------------------- */

#include "surface.hpp"

/* -------------------------------------------------------------------------- *
 *  Interface
 * -------------------------------------------------------------------------- */

class SurfaceWindow
{
public:
    // Interface
    explicit SurfaceWindow() = default;
    virtual ~SurfaceWindow() = 0;

    // Uncopyable
    SurfaceWindow(const SurfaceWindow&) = delete;
    SurfaceWindow& operator=(const SurfaceWindow&) = delete;

    // Apply a given surface at location (x, y)
    virtual void Apply(const Surface &surface_, int x, int y) = 0;

    // Remove all applied surfaces
    virtual void Clean() = 0;

    // Output the current window state
    virtual void Draw() = 0;
};

inline SurfaceWindow::~SurfaceWindow() {}

/* -------------------------------------------------------------------------- */
#endif /* __SURFACE_WINDOW_INTERFACE_HPP__ */