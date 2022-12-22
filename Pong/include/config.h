#ifndef __CONFIG_H__
#define __CONFIG_H__
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- *
 *  Libraries
 * -------------------------------------------------------------------------- */

#include "pong.hpp"
    // using Pong

#include "linux_kbd_device.hpp"
    // using LinuxKbdDevice

#include "term_surface.hpp"
    // using TermSurface

/* -------------------------------------------------------------------------- *
 *  Information
 * -------------------------------------------------------------------------- */

// Used classes
#define GAME_CLASS              Pong
#define LOCAL_KEYBOARD_CLASS    LinuxKbdDevice
#define GRAPHIC_CLASS           TermSurface

// Config
#define CONFIG_WIDTH    65
#define CONFIG_HEIGHT   48
#define CONFIG_NAME     "Pong"

/* -------------------------------------------------------------------------- */
#endif /* __CONFIG_H__ */