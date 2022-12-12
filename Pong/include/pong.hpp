#ifndef __PONG_HPP__
#define __PONG_HPP__
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- *
 *  Libraries
 * -------------------------------------------------------------------------- */

#include <chrono>
    using std::chrono::time_point;
    using std::chrono::steady_clock;
    using std::chrono::duration;

#include <array>
    using std::array;

using std::pair;

#include "game_logic.hpp"

#include "kbd_input.hpp"

/* -------------------------------------------------------------------------- *
 *  Info structure
 * -------------------------------------------------------------------------- */

struct Ball
{
    float x;
    float y;

    float direction_x;
    float direction_y;

    float velocity;
    float acceleration;
};

struct Player
{
    float y;
    size_t score;

    int up_key;
    int down_key;

    KbdInput *device;
};

/* -------------------------------------------------------------------------- *
 *  Class
 * -------------------------------------------------------------------------- */

class Pong : public GameLogic
{
public:
    enum GameState
    {
        Run,
        Pause,
        End
    };

public:
    explicit Pong();
    ~Pong();

    //Uncopyable GameLogic

    void Initialize() override;
    void HandleInput() override;
    void Update() override;
    void Draw(SurfaceWindow *window_) override;
    void Deinitialize() override;
    bool IsRunning() override;

private:
    void RespawnBall();
    void UpdateDelta();

    // Graphic loading
    static Surface s_player_surface;
    static Surface s_ball_surface;
    static Surface s_vertical_line_surface;
    static array<Surface, 10> s_number_surface;
    static array<pair<size_t, size_t>, 2> s_score_x;

    // Player info
    array<Player, 2> m_player;
    Ball m_ball;

    float m_delta;
    pair<time_point<steady_clock>, time_point<steady_clock>> m_timestamp;

    GameState m_state;
    bool is_alive;
};

/* -------------------------------------------------------------------------- */
#endif /* __PONG_HPP__ */