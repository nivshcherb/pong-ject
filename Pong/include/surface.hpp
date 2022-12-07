#ifndef __SURFACE_HPP__
#define __SURFACE_HPP__
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- *
 *  Libraries
 * -------------------------------------------------------------------------- */

#include <array>
    using std::array;

#include <vector>
    using std::vector;

using std::size_t;

/* -------------------------------------------------------------------------- *
 *  Classes
 * -------------------------------------------------------------------------- */

class Pixel
{
public:
    enum Color
    {
        Red = 0,
        Grenn,
        Blue,
        Num = 3
    };

    using Container = array<unsigned char, Color::Num>;

public:
    Pixel(unsigned char red_, unsigned char green_, unsigned char blue_) noexcept;
    Pixel(Container values_) noexcept;
    Pixel() noexcept;

    ~Pixel() = default;

    Pixel(const Pixel&) = default;
    Pixel& operator=(const Pixel&) = default;

    #if __cplusplus > 199711L // c++11 and above
    Pixel(Pixel&&) = default;
    Pixel& operator=(Pixel&&) = default;
    #endif // __cplusplus > 199711L

    // Allow pixel comparison for drawing optimization
    friend bool operator==(const Pixel &lhs_, const Pixel &rhs_);

    // Accessors
    unsigned char& operator[](Color color_);
    unsigned char operator[](Color color_) const;

private:
    /* Members ---------------------------------------------------------- */
    Container m_values;
};

    /* ------------------------------------------------------------------ */

class Surface
{
public:
    using SubContainer = vector<Pixel>;
    using Container = vector<SubContainer>;

public:
    Surface(Container pixels_);
    Surface(size_t width_, size_t height_);

    ~Surface() = default;

    Surface(const Surface&) = default;
    Surface& operator=(const Surface&) = default;

    #if __cplusplus > 199711L /* c++11 and above */
    Surface(Surface&&) = default;
    Surface& operator=(Surface&&) = default;
    #endif /* __cplusplus > 199711L */

    // Accessors
    size_t GetWidth() const noexcept;
    size_t GetHeight() const noexcept;
    vector<Pixel>& operator[](size_t index_);
    const vector<Pixel>& operator[](size_t index_) const;

private:
    Container m_pixels;
};

/* -------------------------------------------------------------------------- */
#endif /* __SURFACE_HPP__ */