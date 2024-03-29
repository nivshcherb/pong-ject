/* -------------------------------------------------------------------------- *
 *  Libraries
 * -------------------------------------------------------------------------- */

#include <stdexcept>
    using std::invalid_argument;

#include "surface.hpp"

/* -------------------------------------------------------------------------- *
 *  Misc.
 * -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- *
 *  Pixel Implementations
 * -------------------------------------------------------------------------- */

Pixel::Pixel(unsigned char red_, unsigned char green_, unsigned char blue_) noexcept :
    m_values{ red_, green_, blue_ }
{
    // Do nothing
}

Pixel::Pixel(Container values_) noexcept :
    m_values(values_)
{
    // Do nothing
}

Pixel::Pixel() noexcept :
    Pixel(0, 0, 0)
{
    // Do nothing
}

unsigned char& Pixel::operator[](Color color_)
{
    return m_values[color_];
}

unsigned char Pixel::operator[](Color color_) const
{
    return m_values[color_];
}

bool operator==(const Pixel &lhs_, const Pixel &rhs_)
{
    return lhs_.m_values == rhs_.m_values;
}

/* -------------------------------------------------------------------------- *
 *  Surface Implementations
 * -------------------------------------------------------------------------- */

Surface::Surface(Container pixels_) :
    m_pixels(pixels_)
{
    if (0 == pixels_.size() || 0 == pixels_.front().size())
    {
        throw invalid_argument("Unable to create surface of size 0...");
    }
}

Surface::Surface(size_t width_, size_t height_, Pixel fill_) :
    Surface(Container(height_, SubContainer(width_, fill_)))
{
    // Do nothing
}

size_t Surface::GetWidth() const noexcept
{
    return m_pixels.front().size();
}

size_t Surface::GetHeight() const noexcept
{
    return m_pixels.size();
}

vector<Pixel>& Surface::operator[](size_t index_)
{
    return m_pixels[index_];
}
const vector<Pixel>& Surface::operator[](size_t index_) const
{
    return m_pixels[index_];
}