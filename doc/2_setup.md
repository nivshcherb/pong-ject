# Setup and Configuration
Create your project directory tree as such:
       
    Pong/
    ├── bin/
    ├── include/
    |   └──config.h
    ├── interface/
    ├── makefile
    ├── obj/
    ├── src/
    └── test/

For now, fill makefile with variables and flags:
ROOT, BIN, TEST, INC, ONJ, SRC, CC, CFLAGS, LDFLAGS

# config.h

**config.h** will be used to provide configurability of the engine
without having to edit the main source.

Add the following define macros to your *config.h* file:
* **GAME_CLASS**
* **LOCAL_KEYBOARD_CLASS**
* **GRAPHIC_CLASS**
* **CONFIG_WIDTH**
* **CONFIG_HEIGHT**
* **CONFIG_NAME**

## Next chapter

[3. Graphics](3_graphics.md)