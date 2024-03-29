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

#include <unordered_map>
    using std::unordered_map;

using std::pair;

#include "game_logic.hpp"

#include "kbd_input.hpp"

#include "endpoint.hpp"

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

enum MessageControl
{
    None,
    ToPause,
    ToContinue,
    ToEnd,
    ToRestart,
    ToQuit
};

struct Message
{
    float ball_x;
    float ball_y;
    float player_y;
    size_t player1_score;
    size_t player2_score;
    MessageControl command;
};

/* -------------------------------------------------------------------------- *
 *  Class
 * -------------------------------------------------------------------------- */

class Pong : public GameLogic
{
public:
    enum GameType
    {
        Local,
        Server,
        Client
    };

    enum GameState
    {
        ChooseType,
        Connect,
        Run,
        Pause,
        End,
        Close
    };

public:
    explicit Pong();
    ~Pong();

    // Uncopyable GameLogic

    void Initialize() override;
    void HandleInput() override;
    void Update() override;
    void Draw(SurfaceWindow *window_) override;
    void Deinitialize() override;
    bool IsRunning() override;

private:
    void RespawnBall();
    void UpdateDelta();
    bool AnyDevice(bool (KbdInput::* test_)(int), int key_);
    void InitializePlayers();
    void Restart();
    void HandleCommand(MessageControl command_);

    // Graphic loading
    static unordered_map<string, Surface> s_surface;

    static array<pair<size_t, size_t>, 2> s_score_x;

    // Player info
    array<Player, 2> m_player;
    Ball m_ball;
    float m_delta;
    pair<time_point<steady_clock>, time_point<steady_clock>> m_timestamp;
    GameState m_state;
    GameType m_type;
    KbdInput *m_local_kbd;
    EndPoint<Message> *m_tcp;
    Message m_send_msg;
    bool m_restart_on_update;
};

/* -------------------------------------------------------------------------- */
#endif /* __PONG_HPP__ */