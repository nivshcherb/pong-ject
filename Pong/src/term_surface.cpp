/* -------------------------------------------------------------------------- *
 *  Libraries
 * -------------------------------------------------------------------------- */

#include <string>
    using std::to_string;

#include <algorithm>
    using std::min;
    using std::max;

#include <stdexcept>
    using std::runtime_error;
    using std::invalid_argument;

#include "term_surface.hpp"

/* -------------------------------------------------------------------------- *
 *  Misc.
 * -------------------------------------------------------------------------- */

#define _LITERAL(x) #x
#define LITERAL(x)  _LITERAL(x)
#define _(x)        x

#define _ESC()  \033
#define _BEL()  \007
#define _CSI()  _ESC()_([)
#define _OSC()  _ESC()_(])

#define ANSI_CURSUR_OFF LITERAL(_CSI()_(?25l))
#define ANSI_CURSUR_ON  LITERAL(_CSI()_(?25h))
#define ANSI_CLEAR      LITERAL(_ESC()_(c))
#define ANSI_NEXT_LINE  LITERAL(_CSI()_(1E))
#define ANSI_TO_START   LITERAL(_CSI()_(H))

inline string ANSI_TITLE(const string title_)
{
    return LITERAL(_OSC()_(0;)) + title_ + LITERAL(_BEL());
}

inline string ANSI_SIZE(size_t width_, size_t height_)
{
    return LITERAL(_CSI()8;) + to_string(height_)
            + LITERAL(;) + to_string(width_) + LITERAL(t);
}

inline const char *ANSI_FG_RGB(unsigned char red_, unsigned char green_, unsigned char blue_)
{
    static const char *pattern = LITERAL(_CSI()) "38;2;%d;%d;%dm";
    static char buffer[32] = { 0 };

    sprintf(buffer, pattern, red_, green_, blue_);

    return buffer;
}

inline const char *ANSI_BG_RGB(unsigned char red_, unsigned char green_, unsigned char blue_)
{
    static const char *pattern = LITERAL(_CSI()) "48;2;%d;%d;%dm";
    static char buffer[32] = { 0 };

    sprintf(buffer, pattern, red_, green_, blue_);

    return buffer;
}

/* -------------------------------------------------------------------------- *
 *  TermSurface Implementations
 * -------------------------------------------------------------------------- */

TermSurface::TermSurface(const string &title_, size_t width_, size_t height_) :
    m_prev_attr{ 0 },
    m_surface(width_, height_),
    m_prev_surface(m_surface)
{
    if (0 == width_ || 0 == height_)
    {
        throw(invalid_argument("Unable to create window of size 0..."));
    }

    // Set termios attributes
    Termios curr_attr{ 0 };
    if (0 > tcgetattr(fileno(stdout), &curr_attr))
    {
        throw(runtime_error("Unable to get terminal attributes..."));
    }
    m_prev_attr = curr_attr;

    curr_attr.c_lflag &= ~ECHO;    // Diable echo
    curr_attr.c_lflag &= ~ICANON;  // Disable input buffering
    curr_attr.c_cc[VMIN] = 0;
    curr_attr.c_cc[VTIME] = 0;

    if (0 > tcsetattr(fileno(stdout), TCSANOW, &curr_attr))
    {
        throw(runtime_error("Unable to set terminal attributes..."));
    }

    // Set ANSI attributes
    cout << ANSI_TITLE(title_);
    cout << ANSI_CURSUR_OFF;
    cout << ANSI_SIZE(GetWidth(), GetHeight() / 2);
    cout << ANSI_CLEAR;
}

TermSurface::~TermSurface()
{
    // Revert termios attributes
    tcsetattr(fileno(stdout), TCSAFLUSH, &m_prev_attr);

    // Revert ANSI attributes
    cout << ANSI_CURSUR_ON;
    cout << ANSI_CLEAR;
}

void TermSurface::Apply(const Surface *surface_, int x_, int y_)
{
    // Compute surfaces overlap
    size_t from_x = max(0, x_);
    size_t from_y = max(0, y_);
    size_t to_x = min(GetWidth(), x_ + surface_->GetWidth());
    size_t to_y = min(GetHeight(), y_ + surface_->GetHeight());

    // Replace pixels with overlapping pixels
    for (size_t y = from_y; y < to_y; ++y)
    {
        for (size_t x = from_x; x < to_x; ++x)
        {
            m_surface[y][x] = (*surface_)[y - y_][x - x_];
        }
    }
}

void TermSurface::Clean()
{
    // Set new default surface
    m_surface = Surface(GetWidth(), GetHeight());
}

void TermSurface::Draw()
{
    constexpr static size_t DRAW_SIZE = 44;

    cout << ANSI_TO_START;

    // For each pair of pixels: (height / 2) * width
    for (size_t y = 0; y < GetHeight(); y += 2)
    {
        for (size_t x = 0; x < GetWidth(); ++x)
        {
            if (m_surface[y][x] == m_prev_surface[y][x] &&
                m_surface[y + 1][x] == m_prev_surface[y + 1][x])
            {
                continue;
            }

            // Print top pixel in foreground and bottom pixel in background
            cout    << ANSI_FG_RGB( m_surface[y][x][Pixel::Color::Red],
                                    m_surface[y][x][Pixel::Color::Grenn],
                                    m_surface[y][x][Pixel::Color::Blue])
                    << ANSI_BG_RGB( m_surface[y + 1][x][Pixel::Color::Red],
                                    m_surface[y + 1][x][Pixel::Color::Grenn],
                                    m_surface[y + 1][x][Pixel::Color::Blue])
                    << "▀";
        }

        cout << ANSI_NEXT_LINE;
    }

    // Complete printing to terminal
    cout << flush;

    m_prev_surface = m_surface;
}

size_t TermSurface::GetWidth() const
{
    return m_surface.GetWidth();
}

size_t TermSurface::GetHeight() const
{
    return m_surface.GetHeight();
}