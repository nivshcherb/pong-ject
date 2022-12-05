#include <iostream>
#include <chrono>
#include "termwin.hpp"

#include <linux/input.h>

using namespace std;
using namespace chrono;

int main()
{
    TermWin win("test", 100, 50);

    Surface surface[2]{
        {
            vector<vector<Pixel>>(100, vector<Pixel>(100, { 0, 0, 255 })),
            100, 100
        },
        {
            vector<vector<Pixel>>(100, vector<Pixel>(100, { 255, 0, 0 })),
            100, 100
        }
    };

    auto timestamp = steady_clock::now();
    auto prev_time = timestamp;
    pair<size_t, size_t> count{ 0, 10};
    duration<float> calc(0);
    duration<float> avg(0);

    bool current = false;
    while (false)
    {
        win.Clean();
        win.Apply(surface[current], 0, 0);
        win.Draw();

        current = !current;

        timestamp = steady_clock::now();

        calc += timestamp - prev_time;
        prev_time = timestamp;

        ++count.first;
        if (count.first == count.second)
        {
            avg = calc / count.second;
            calc = duration<float>(0);
            count.first = 0;
        }
        cout << avg.count() << endl;
    }

    return 0;
}