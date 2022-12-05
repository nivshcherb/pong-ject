#ifndef __SURFACE_HPP__
#define __SURFACE_HPP__
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- *
 *  Libraries
 * -------------------------------------------------------------------------- */

#include <vector>
    using std::vector;

/* -------------------------------------------------------------------------- *
 *  Misc.
 * -------------------------------------------------------------------------- */

typedef struct pixel Pixel;
typedef struct surface Surface;

/* -------------------------------------------------------------------------- *
 *  Structures
 * -------------------------------------------------------------------------- */

struct pixel
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

struct surface
{
    vector<vector<Pixel>> pixels;
    size_t height;
    size_t width;
};

inline Surface GetEmptySurface(size_t width, size_t height)
{
    return {
        vector<vector<Pixel>>(height, vector<Pixel>(width, { 0 })),
        height, width
    };
}

/* -------------------------------------------------------------------------- */
#endif // __SURFACE_HPP__