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

#define IP_ADDRESS      "127.0.0.1"
#define PORT            5000

// Gameplay attributes
#define PLAYER_SPEED        20.0f
#define PLAYER_LENGTH       5
#define BALL_INIT_VELOCITY  10.0f
#define BALL_ACCELERATION   1.0f
#define WIN_SCORE           1

// Local input control
#define PLAYER_1_UP         KEY_UP
#define PLAYER_1_DOWN       KEY_DOWN
#define PLAYER_2_UP         KEY_Q
#define PLAYER_2_DOWN       KEY_A
#define PAUSE_KEY           KEY_P
#define EXIT_KEY            KEY_ESC
#define RESTART_KEY         KEY_R

/* -------------------------------------------------------------------------- */
#endif /* __CONFIG_H__ */