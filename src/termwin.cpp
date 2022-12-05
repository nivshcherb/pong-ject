/* -------------------------------------------------------------------------- *
 *  Libraries
 * -------------------------------------------------------------------------- */

#include <iostream>
    using std::cout;
    using std::flush;

#include <algorithm>
    using std::min;
    using std::max;

#include <stdexcept>
    using std::runtime_error;

#include "termwin.hpp"

/* -------------------------------------------------------------------------- *
 *  Misc.
 * -------------------------------------------------------------------------- */

constexpr size_t DRAW_SIZE = 44;

/* -------------------------------------------------------------------------- *
 *  TermWin Implementation
 * -------------------------------------------------------------------------- */

TermWin::TermWin(string title, size_t width, size_t height) :
    m_surface(GetEmptySurface(width, height)),
    m_attributes{ 0 }
{
    // Set termios attributes
    Termios curr_attr{ 0 };
    if (0 > tcgetattr(fileno(stdout), &curr_attr))
    {
        throw(runtime_error("Unable to get terminal attributes"));
    }
    m_attributes = curr_attr;

    curr_attr.c_lflag &= ~ECHO;    // Diable echo
    curr_attr.c_lflag &= ~ICANON;  // Disable input buffering
    curr_attr.c_cc[VMIN] = 0;
    curr_attr.c_cc[VTIME] = 0;

    if (0 > tcsetattr(fileno(stdout), TCSANOW, &curr_attr))
    {
        throw(runtime_error("Unable to set terminal attributes"));
    }

    // ANSI escape code attributes
    cout << ESC_TITLE(title);
    cout << ESC_BLINK_OFF;
    cout << ESC_RESIZE(height, width);
    cout << ESC_CLEAR;
}

TermWin::~TermWin()
{
    // Reset termios attributes
    tcsetattr(fileno(stdout), TCSAFLUSH, &m_attributes);

    // Reset ANSI escape code attributes
    cout << ESC_BLINK_ON;
    cout << ESC_CLEAR;
}

void TermWin::Apply(const Surface &surface, int x, int y)
{
    // Compute boundaries
    size_t from_y = max(0, y);
    size_t from_x = max(0, x);
    size_t to_y = min(m_surface.height, y + surface.height);
    size_t to_x = min(m_surface.width, x + surface.width);

    // Write to each overlapping pixel
    for (size_t yi = from_y; yi < to_y; ++yi)
    {
        for (size_t xi = from_x; xi < to_x; ++xi)
        {
            m_surface.pixels[yi][xi] = surface.pixels[yi - y][xi - x];
        }
    }
}

void TermWin::Clean()
{
    m_surface = GetEmptySurface(m_surface.width, m_surface.height);
}

void TermWin::Draw()
{
    constexpr static size_t DRAW_SIZE = 44;

    cout << ESC_CURSOR(0, 0);

    // For each pair of pixels; (height / 2) * width
    for (size_t y = 0; y < m_surface.height; y += 2)
    {
        for (size_t x = 0; x < m_surface.width; ++x)
        {
            // Print top pixel in foreground and bottom pixel in background
            char buffer[DRAW_SIZE]{ 0 };
            sprintf(buffer, "\033[38;2;%d;%d;%dm\033[48;2;%d;%d;%dm▀\033[0m",
            m_surface.pixels[y][x].red,
            m_surface.pixels[y][x].green,
            m_surface.pixels[y][x].blue,
            m_surface.pixels[y + 1][x].red,
            m_surface.pixels[y + 1][x].green,
            m_surface.pixels[y + 1][x].blue);
            cout << buffer;

        }
        cout << ESC_NEWLINE;
    }
    cout << ESC_RESET;
    cout << flush;
}





