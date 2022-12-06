# Graphics

---

Create 2 classes, **Pixel** and **Surface**.
Class   |   Fields
:---    |   :---
Pixel   |   Red, green and blue
Surface |   2D matrix of *Pixel*s
These classed do not have much required functionality, and are mostly used to encapsulate the information. Make sure you have get/set access the fields.
You can add methods as you see fit.

---

Create **SurfaceWindow** interface.
*SurfaceWindow* describes the functionality required from any class that want to draw our game (not necessarily using Linux termianl).
Method  |   Description
:---    |   :---
Apply   |   Apply a given surface at location (x, y).
Clean   |   Remove all applied surfaces.
Draw    |   Output the current window state.
Make sure SurfaceWindow is as portable as possible.

---

Create **TermSurface** class that implements *SurfaceWindow*.
For our implementation, we will use **Termios** and **ANSI escape codes**, and our *Surface* class.

> The termios functions describe a general terminal interface that is provided to control asynchronous communications ports. [*](https://en.wikipedia.org/wiki/ANSI_escape_code)

> ANSI escape sequences are a standard for in-band signaling to control cursor location, color, font styling, and other options on video text terminals and terminal emulators. [*](https://man7.org/linux/man-pages/man3/termios.3.html)

Method  |   Description
:---    |   :---
Ctor    |   Disable terminal echo, input buffering and cursur, resize window, change the title and clear the terminal.
Dtor    |   Revert all changes done in Ctor().
Apply   |   Replace the pixels held by TermSurface with pixels from the given Surface. The user may apply a surface completly/partly outside the boundaries, so handle accordingly.
Clean   |   Set the pixels held by TermSurface to some default value.
Draw    |   Print each pixel with it's color using ANSI escape code. Setting the background color and printing a space character should do the trick.

Improve the visuals and your *Draw()* method by including 2 pixels in each character. ▀ is you friend.

---

Test TermSurface and make sure everything is working correctly. Finding bugs at this point will save you time when the project becomes more complicated.


In a loop, draw a surface on the entire sceen. Choose a different colored surface each loop. Compute the average time Draw() takes. Try to zoom out in the terminal to fit a many pixels as possible.

Which of the 3 functions in the loop has the worst preformance? Why?

This type of “graphics” isn’t optimal, but it is enough for our simple game.
Later on, we'll skip the *Draw()* function most of the time, but this does not concern TermSurface at all.
