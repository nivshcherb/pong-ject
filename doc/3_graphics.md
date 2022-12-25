# Graphics

<img src="https://i.kym-cdn.com/photos/images/original/001/018/866/e44.png" alt="graphic" width="400"/>

One of the steps inside the gameloop is *Draw()* which will tell some class how to output.
Our engine will use a class, *Surface*, to store and edit a 2D array of pixels.

## Surface

In order to save information about pixels, we will create 2 classes:
**Pixel** and **Surface**.

*Pixel* is optional and used to store the values for red, green and blue
instead of handling *Surface* as a 3D array. Each value have a range of 0 to 255 (char).

*Surface* is the main object used for drawing.
Use 2D vector to store the pixels and provide access to each pixel individually.

You can add constructors and methods to these classes as you see fit.

As a bonus, add an option to load an image as *Surface* from a given path.
I used the format ".raw" as it is the simplest one and does not require any libraries.

## SurfaceWindow

Since we do not want the engine to depend on the operation system, we would like to write
some interface and specify what methods are available to the game engine regardless of implementation.

Create *SurfaceWindow* interface with the following public methods:

Method  |   Description
:---    |   :---
Apply   |   Given a *Surface* and a location (x, y), apply the surface to the window
Clean   |   Remove all *Surface*s added with *Apply()*.
Draw    |   Output the current window.

Make sure you do not depend on any specification other than the use of *Surface*.

## TermSurface

We will implement *SurfaceWindow* using ubuntu's default terminal. In order to
do so, we will use *Termios* and *ANSI escape codes* to configurate the terminal
and print out the stored pixels as characters with background / foreground color.

> The termios functions describe a general terminal interface that is provided to control asynchronous communications ports. [*](https://en.wikipedia.org/wiki/ANSI_escape_code)

> ANSI escape sequences are a standard for in-band signaling to control cursor location, color, font styling, and other options on video text terminals and terminal emulators. [*](https://man7.org/linux/man-pages/man3/termios.3.html)

Create *TermSurface* that implements *SurfaceWindow* for ubuntu.

Method  |   Description
:---    |   :---
Ctor    |   Use both *Termios* and *ANSI escape codes* to disable echo, input buffering and cursor, change the title and clear the terminal completely.
Dtor    |   Revert all changes done in *Ctor()*.
Apply   |   Given a *Surface* and a location (x, y), replace the stored pixels with the pixels from the *Surface*. Consider the end cases that might happen.
Clean   |   Set the stored pixels to some default value.
Draw    |   Print each stored pixel using *ANSI escape code*.
Simply changing the background color and priting space character should do the trick.


## Performance

Try running some tests on *TermSurface*. Check as many end cases you can think of.
Finding bugs at this point will substantially reduce complications later on.

After you feel everything is working correctly, check the performance of you *Draw()* function
by drawing 2 *Surface*s of different color on the entire screen one after the other
and calculating the average time each *Draw()* takes to complete.

Find ways to improve *Draw()*. 2 rather simple solutions are:

1. Using both background and foreground colors to print 2 pixels at a time. Replace the space character with ▀.
2. Only print pixels that were changed since the last *Draw()* call.

Compare the performance of *Draw()* before and after your improvements.


Consider reading about *OpenGL* and *SDL* for a better interface for outputing graphics.

## Relevant Files

- [include/surface.hpp](../Pong/include/surface.hpp)
- [interface/surface_window.hpp](../Pong/interface/surface_window.hpp)
- [include/term_surface.hpp](../Pong/include/term_surface.hpp)
- [src/surface.cpp](../Pong/src/surface.cpp)
- [src/term_surface.cpp](../Pong/src/term_surface.cpp)
- [test/surface_test.cpp](../Pong/test/surface_test.cpp)

## Next chapter

[4. Keyboard](4_keyboard.md)