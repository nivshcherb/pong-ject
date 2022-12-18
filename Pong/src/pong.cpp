/* -------------------------------------------------------------------------- *
 *  Libraries
 * -------------------------------------------------------------------------- */

#include <algorithm>
    using std::min;
    using std::max;
    using std::abs;

#include <cmath>
    using std::sqrt;

#include <cstdlib>
    using std::rand;

#include "pong.hpp"

#include "tcp.hpp"

#include "config.h"

/* -------------------------------------------------------------------------- *
 *  Misc
 * -------------------------------------------------------------------------- */

static const Pixel W( 100, 100, 100 );
static const Pixel B( 0, 0, 0 );

/* -------------------------------------------------------------------------- *
 *  Pong Implementations
 * -------------------------------------------------------------------------- */

Pong::Pong() :
    m_player(),
    m_ball{},
    m_delta(0),
    m_timestamp{ steady_clock::now(), steady_clock::now() }
{
    // Do nothing
}

Pong::~Pong() {}

void Pong::Initialize()
{
    m_state = GameState::ChooseType;

    RespawnBall();
    m_player = array<Player, 2>();

    // Create local keyboard
    m_local_kbd = new LinuxKbdDevice;
}

void Pong::HandleInput()
{
    UpdateDelta();

    switch (m_state)    // State machine
    {
        case GameState::ChooseType:
            if (m_local_kbd->IsPressed(KEY_L))
            {
                m_type = GameType::Local;
            }



            break;

        case GameState::Connect:
            break;

        case GameState::Run:
            break;

        case GameState::Pause:
            break;

        case GameState::End:
            break;

        case GameState::Close:
            break;
    }

    m_player[0].device->Update();

    if (AnyDevice(&KbdInput::IsPressed, EXIT_KEY))
    {
        m_state = GameState::Close;
        return;
    }

    if (AnyDevice(&KbdInput::IsPressed, RESTART_KEY))
    {
        Deinitialize();
        Initialize();
        return;
    }

    if (AnyDevice(&KbdInput::IsPressed, PAUSE_KEY))
    {
        // use state machine to handle pause
        static GameState state_maping[] =
        {
            /* Run      => */   GameState::Pause,
            /* Pause    => */   GameState::Run,
            /* End      => */   GameState::End,
            /* Closing  => */   GameState::Close
        };

        m_state = state_maping[m_state];
    }

    // For a running game, accept input
    if (GameState::Run != m_state) return;

    for (size_t i = 0; i < 2; ++i)
    {
        // Get movement direction
        int direction =
            (m_player[i].device->IsDown(m_player[i].down_key)) -    // pos sign
            (m_player[i].device->IsDown(m_player[i].up_key));       // neg sign

        m_player[i].y += direction * m_delta * PLAYER_SPEED;
    }
}

void Pong::Update()
{
    // Do not update if game is not running
    if (GameState::Run != m_state) return;

    m_ball.x += m_ball.direction_x * m_ball.velocity * m_delta;
    m_ball.y += m_ball.direction_y * m_ball.velocity * m_delta;
    m_ball.velocity += m_ball.acceleration * m_delta;

    // Snap players to boundaries
    for (size_t i = 0; i < 2; ++i)
    {
        m_player[i].y = max(0.0f,
                            min(m_player[i].y,
                                float(CONFIG_HEIGHT - PLAYER_LENGTH)));
    }

    // Bounce of upper and lower boundaries

    int new_direction = (m_ball.y < 0) -                    // downwards
                        (m_ball.y > CONFIG_HEIGHT);  // upwards

    if (0 != new_direction)
    {
        m_ball.direction_y = new_direction * abs(m_ball.direction_y);
    }

    // Bounce of players
    if ((m_ball.x > CONFIG_WIDTH - 1) &&            // Right player column
        (m_player[0].y < m_ball.y) &&               // In player's boundary
        (m_ball.y < m_player[0].y + PLAYER_LENGTH))
    {
        m_ball.direction_x = -abs(m_ball.direction_x);
    }
    else if ((m_ball.x < 1) &&                      // Left player column
        (m_player[1].y < m_ball.y) &&               // In player's boundary
        (m_ball.y < m_player[1].y + PLAYER_LENGTH))
    {
        m_ball.direction_x = abs(m_ball.direction_x);
    }
    
    // Score when out of boundary
    else if (m_ball.x < 0)
    {
        ++m_player[0].score;
        RespawnBall();
    }
    else if (m_ball.x > CONFIG_WIDTH)
    {
        ++m_player[1].score;
        RespawnBall();
    }

    // Test end of game condition
    if ((WIN_SCORE == m_player[0].score) ||
        (WIN_SCORE == m_player[1].score))
    {
        m_state = GameState::End;
    }
}

void Pong::Draw(SurfaceWindow *window_)
{
    window_->Clean();   // Start

    // Players score
    for (size_t i = 0; i < 2; ++i)
    {
        window_->Apply(&s_number_surface[m_player[i].score % 10],
                        s_score_x[i].second, 1);
        window_->Apply(&s_number_surface[m_player[i].score / 10],
                        s_score_x[i].first, 1);
    }

    // Vertical bar
    window_->Apply(&s_vertical_line_surface, CONFIG_WIDTH / 2, 0);

    // Player 1
    window_->Apply(&s_player_surface, 0, (int)m_player[1].y);

    // Player 2
    window_->Apply(&s_player_surface, CONFIG_WIDTH - 1, (int)m_player[0].y);

    // Ball
    window_->Apply(&s_ball_surface, (int)m_ball.x, (int)m_ball.y);

    window_->Draw();    // End
}

void Pong::Deinitialize()
{
    delete m_player[0].device;
}

bool Pong::IsRunning()
{
    return (GameState::Close != m_state);
}

void Pong::RespawnBall()
{
    m_ball.acceleration = BALL_ACCELERATION;
    m_ball.direction_x = ((rand() % 2) ? 1 : -1) / sqrt(2);
    m_ball.direction_y = ((rand() % 2) ? 1 : -1) / sqrt(2);
    m_ball.velocity = BALL_INIT_VELOCITY;
    m_ball.x = CONFIG_WIDTH / 2;
    m_ball.y = CONFIG_HEIGHT / 2;
}

void Pong::UpdateDelta()
{
    m_timestamp.second = m_timestamp.first;
    m_timestamp.first = steady_clock::now();
    m_delta = duration<float>(m_timestamp.first - m_timestamp.second).count();
}

bool Pong::AnyDevice(bool (KbdInput::* test_)(int), int key_)
{
    for (Player &player : m_player)
    {
        if ((player.device->*test_)(key_)) return true;
    }
    return false;
}

void Pong::InitializePlayers()
{
    // Default as local players
    m_player[0] = { float(CONFIG_HEIGHT - PLAYER_LENGTH) / 2,
                    0, PLAYER_1_UP,  PLAYER_1_DOWN, m_local_kbd };
    m_player[1] = { float(CONFIG_HEIGHT - PLAYER_LENGTH) / 2,
                    0, PLAYER_2_UP, PLAYER_2_DOWN, m_local_kbd };

    switch (m_type)
    {
        case GameType::Local:
            break;
        
        case GameType::Client:
            m_player[0].device = nullptr;   // Disable input
            m_tcp = new TcpClient<Message, Message>(IP_ADDRESS, PORT);
            break;

        case GameType::Server:
            m_player[1].device = nullptr;   // Disable input
            m_tcp = new TcpServer<Message, Message>(PORT);
            break;
    }
}

/* -------------------------------------------------------------------------- *
 *  Pong Static Variables
 * -------------------------------------------------------------------------- */

Surface Pong::s_player_surface(1, PLAYER_LENGTH, Pixel(255, 255, 255));

Surface Pong::s_ball_surface(1, 1, Pixel(255, 255, 255));

Surface Pong::s_vertical_line_surface(1, CONFIG_HEIGHT, W);

array<Surface, 10> Pong::s_number_surface{
    Surface({   // 0
        { W, W, W },
        { W, B, W },
        { W, B, W },
        { W, B, W },
        { W, W, W } }),
    Surface({   // 1
        { B, W, B },
        { B, W, B },
        { B, W, B },
        { B, W, B },
        { B, W, B } }),
    Surface({   // 2
        { W, W, W },
        { B, B, W },
        { W, W, W },
        { W, B, B },
        { W, W, W } }),
    Surface({   // 3
        { W, W, W },
        { B, B, W },
        { W, W, W },
        { B, B, W },
        { W, W, W } }),
    Surface({   // 4
        { W, B, W },
        { W, B, W },
        { W, W, W },
        { B, B, W },
        { B, B, W } }),
    Surface({   // 5
        { W, W, W },
        { W, B, B },
        { W, W, W },
        { B, B, W },
        { W, W, W } }),
    Surface({   // 6
        { W, W, W },
        { W, B, B },
        { W, W, W },
        { W, B, W },
        { W, W, W } }),
    Surface({   // 7
        { W, W, W },
        { B, B, W },
        { B, B, W },
        { B, B, W },
        { B, B, W } }),
    Surface({   // 8
        { W, W, W },
        { W, B, W },
        { W, W, W },
        { W, B, W },
        { W, W, W } }),
    Surface({   // 9
        { W, W, W },
        { W, B, W },
        { W, W, W },
        { B, B, W },
        { W, W, W } }),
};

array<pair<size_t, size_t>, 2> Pong::s_score_x{{
    { CONFIG_WIDTH / 2 + 2, CONFIG_WIDTH / 2 + 6 },
    { CONFIG_WIDTH / 2 - 8, CONFIG_WIDTH / 2 - 4 }
}};
