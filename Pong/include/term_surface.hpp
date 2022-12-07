#ifndef __TERM_SURFACE_HPP__
#define __TERM_SURFACE_HPP__
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- *
 *  Libraries
 * -------------------------------------------------------------------------- */

#include <string>
    using std::string;

#include <iostream>
    using std::cout;
    using std::flush;

#include <termios.h>
    typedef struct termios Termios;

#include "surface_window.hpp"

/* -------------------------------------------------------------------------- *
 *  Classes
 * -------------------------------------------------------------------------- */

class TermSurface : public SurfaceWindow
{
public:
    ~TermSurface();

    // Uncopyable SurfaceWindow

    // Override methods
    void Apply(const Surface &surface_, int x_, int y_) override;
    void Clean() override;
    void Draw() override;

    // Accessors
    size_t GetWidth() const;
    size_t GetHeight() const;

    // Singleton access
    static TermSurface *Init(const string &title_, size_t width_, size_t height_);

private:
    explicit TermSurface(const string &title_, size_t width_, size_t height_);

    // Members
    Termios m_prev_attr;
    Surface m_surface;

    Surface m_prev_surface;
};


/* -------------------------------------------------------------------------- */
#endif /* __TERM_SURFACE_HPP__ */