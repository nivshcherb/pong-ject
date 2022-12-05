#ifndef __TERMWIN_HPP__
#define __TERMWIN_HPP__
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- *
 *  Libraries
 * -------------------------------------------------------------------------- */

#include <string>
    using std::string;

#include <termios.h>
    typedef struct termios Termios;

#include "window.hpp"

/* -------------------------------------------------------------------------- *
 *  TermWin Class
 * -------------------------------------------------------------------------- */

class TermWin : public Window
{
    uncopyable(TermWin);

public:
    TermWin(string title, size_t width, size_t height);
    ~TermWin();

    // Override
    void Apply(const Surface &surface, int x, int y) override;
    void Clean() override;
    void Draw() override;

private:
    /* members ---------------------------------------------------------- */
    Surface m_surface;
    Termios m_attributes;
};

/* -------------------------------------------------------------------------- */
#endif // __TERMWIN_HPP__