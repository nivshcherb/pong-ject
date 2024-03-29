/* -------------------------------------------------------------------------- *
 *  Libraries
 * -------------------------------------------------------------------------- */

#include <iostream>
#include <chrono>

#include "surface.hpp"
#include "term_surface.hpp"

using namespace std;
using namespace chrono;

/* -------------------------------------------------------------------------- *
 *  Misc
 * -------------------------------------------------------------------------- */

constexpr static size_t LOOPS = 100;

/* -------------------------------------------------------------------------- *
 *  Main
 * -------------------------------------------------------------------------- */

int main()
{
    SurfaceWindow *win = new TermSurface("Test", 50, 52);

    Surface rec[2]{
        Surface::Container(50, Surface::SubContainer(50, Pixel(255, 0, 0))),
        Surface::Container(50, Surface::SubContainer(50, Pixel(0, 0, 255)))
        };

    bool current_rec = false;
    pair<size_t, size_t> count{0, LOOPS};
    pair<time_point<steady_clock>, time_point<steady_clock>> timestamp;
    duration<float> avg;
    duration<float> delta_sum;

    while (true)
    {
        timestamp.first = steady_clock::now();

        win->Clean();
        win->Apply(rec + current_rec, 0, 0);
        win->Draw();

        timestamp.second = steady_clock::now();

        delta_sum += timestamp.second - timestamp.first;
        ++count.first;
        if (count.first == count.second)
        {
            avg = delta_sum / LOOPS;
            delta_sum = duration<float>(0);
            count.first = 0;
            cout << "\033[0m" << "delta: " << avg.count();
        }

        //current_rec = !current_rec;
    }

    delete win;

    return 0;
}
