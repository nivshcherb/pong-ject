/* -------------------------------------------------------------------------- *
 *  Libraries
 * -------------------------------------------------------------------------- */

#include <iostream> // TESTING ONLY
    using std::cout;
    using std::endl;

#include <fstream>
    using std::fstream;
    using std::ios_base;

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
 *  Configuration
 * -------------------------------------------------------------------------- */

static const struct
{
    // Tcp connection
    const char *ip = "127.0.0.1";
    size_t port = 5000;

    // Gameplay attributes
    struct
    {
        float velocity = 10.0f;
        float acceleration = 1.0f;
    } ball;

    struct
    {
        float speed = 20.0f;
        size_t length = 5;

        struct
        {
            int up = KEY_UP;
            int down = KEY_DOWN;
        } first;

        struct
        {
            int up = KEY_Q;
            int down = KEY_A;
        } second;

    } player;
    size_t win_score = 1;

    struct
    {
        int pause = KEY_P;
        int exit = KEY_ESC;
        int restart = KEY_R;
        
        int local_choise = KEY_L;
        int server_choise = KEY_S;
        int client_choise = KEY_C;
    } key;

    string resource_dir = "./resources/";

} CONFIG;

/* -------------------------------------------------------------------------- *
 *  Misc
 * -------------------------------------------------------------------------- */

static const Pixel W( 255, 255, 255 );
static const Pixel G( 100, 100, 100 );
static const Pixel B( 0, 0, 0 );

/* -------------------------------------------------------------------------- *
 *  Pong Implementations
 * -------------------------------------------------------------------------- */

Pong::Pong() :
    m_player(),
    m_ball{0},
    m_delta(0),
    m_timestamp{ steady_clock::now(), steady_clock::now() },
    m_state(GameState::ChooseType),
    m_type(GameType::Local),
    m_local_kbd(nullptr),
    m_tcp(nullptr),
    m_send_msg{0},
    m_restart_on_update(false)
{
    // Do nothing
}

Pong::~Pong() {}

void Pong::Initialize()
{
    m_state = GameState::ChooseType;
    m_restart_on_update = false;

    RespawnBall();
    m_player = array<Player, 2>();

    // Create local keyboard
    m_local_kbd = new LinuxKbdDevice;
}

void Pong::HandleInput()
{
    m_local_kbd->Update();

    switch (m_state)    // State machine
    {
        case GameState::ChooseType:
            if (m_local_kbd->IsPressed(CONFIG.key.local_choise))
            {   // Local play
                m_type = GameType::Local;
                m_state = GameState::Run;
            }
            else if (m_local_kbd->IsPressed(CONFIG.key.server_choise))
            {   // Online play
                m_type = GameType::Server;  
                m_state = GameState::Connect;
            }
            else if (m_local_kbd->IsPressed(CONFIG.key.client_choise))
            {   // Online play
                m_type = GameType::Client;  
                m_state = GameState::Connect;
            }
            if (m_state != GameState::ChooseType)
            {
                InitializePlayers();
            }
            break;

        case GameState::Connect:
            if (m_local_kbd->IsPressed(CONFIG.key.exit))
            {   // Quit connection
                m_state = GameState::Close; 
            }
            break;

        case GameState::Run:
            if (m_local_kbd->IsPressed(CONFIG.key.pause))
            {   // Pause game
                m_state = GameState::Pause;
                m_send_msg.command = MessageControl::ToPause;
            }
            else if (m_local_kbd->IsPressed(CONFIG.key.exit))
            {   // Quit game
                m_state = GameState::Close;
                m_send_msg.command = MessageControl::ToQuit;
            }
            else if (m_local_kbd->IsPressed(CONFIG.key.restart))
            {   // Restart game
                m_restart_on_update = true;
                m_send_msg.command = MessageControl::ToRestart;
            }
            else for (size_t i = 0; i < m_player.size(); ++i)
            {   // Players input
                if (m_player[i].device == nullptr) continue;    // Skip for online

                // Move player acording to input
                int direction =
                    (m_player[i].device->IsDown(m_player[i].down_key)) -    // pos
                    (m_player[i].device->IsDown(m_player[i].up_key));       // neg
                m_player[i].y += direction * m_delta * CONFIG.player.speed;
            }
            break;

        case GameState::Pause:
            if (m_local_kbd->IsPressed(CONFIG.key.pause))
            {   // Continue game
                m_state = GameState::Run;
                m_send_msg.command = MessageControl::ToContinue;
            }
            else if (m_local_kbd->IsPressed(CONFIG.key.exit))
            {   // Quit game
                m_state = GameState::Close;
                m_send_msg.command = MessageControl::ToQuit;
            }
            else if (m_local_kbd->IsPressed(CONFIG.key.restart))
            {   // Restart game
                m_restart_on_update = true;
                m_send_msg.command = MessageControl::ToRestart;
            }
            break;

        case GameState::End:
            if (m_local_kbd->IsPressed(CONFIG.key.restart))
            {   // Restart game
                m_restart_on_update = true;
                m_send_msg.command = MessageControl::ToRestart;
            }
            else if (m_local_kbd->IsReleased(CONFIG.key.exit))
            {   // Quit game
                m_state = GameState::Close;
                m_send_msg.command = MessageControl::ToQuit;
            }
            break;

        case GameState::Close:
            // Do nothing
            break;
    }
}

void Pong::Update()
{
    UpdateDelta();

    int new_direction = 0;
    switch (m_state)
    {
        case GameState::ChooseType:
            // Do nothing
            break;

        case GameState::Connect:
            m_tcp->Connect();
            if (m_tcp->GetState() == ConnectionState::Connected)
            {
                m_state = GameState::Run;
            }
            break;

        case GameState::Run:
            // Snap players to boundaries
            for (size_t i = 0; i < 2; ++i)
            {
                m_player[i].y = max(0.0f,
                                    min(m_player[i].y,
                                        float(CONFIG_HEIGHT - CONFIG.player.length)));
            }

            // Bounce of players
            if ((m_ball.x > CONFIG_WIDTH - 1) &&    // Right player column
                (m_player[0].y < m_ball.y) &&       // In player's boundary
                (m_ball.y < m_player[0].y + CONFIG.player.length))
            {
                m_ball.direction_x = -abs(m_ball.direction_x);
            }
            else if ((m_ball.x < 1) &&              // Left player column
                (m_player[1].y < m_ball.y) &&       // In player's boundary
                (m_ball.y < m_player[1].y + CONFIG.player.length))
            {
                m_ball.direction_x = abs(m_ball.direction_x);
            }

            if (m_type == GameType::Client)
            {   
                break;  // Get ball properties and score from server
            }

            // Move ball
            m_ball.x += m_ball.direction_x * m_ball.velocity * m_delta;
            m_ball.y += m_ball.direction_y * m_ball.velocity * m_delta;
            m_ball.velocity += m_ball.acceleration * m_delta;

            // Ball bounce off upper and lower boundaries
            new_direction = (m_ball.y < 0) -                    // downwards
                            (m_ball.y > CONFIG_HEIGHT);         // upwards
            if (0 != new_direction)
            {
                m_ball.direction_y = new_direction * abs(m_ball.direction_y);
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
            if ((CONFIG.win_score == m_player[0].score) ||
                (CONFIG.win_score == m_player[1].score))
            {
                m_state = GameState::End;
                m_send_msg.command = MessageControl::ToEnd;
                break;
            }
            break;
        
        case GameState::Pause:
            // Do nothing
            break;

        case GameState::End:
            // Do nothing
            break;

        case GameState::Close:
            // Handle at the end
            break;
    }

    if (m_state == GameState::Connect)  return; // Skip is not connected

    if (m_type == GameType::Server)
    {   
        // Build message
        m_send_msg.ball_x = m_ball.x;
        m_send_msg.ball_y = m_ball.y;
        m_send_msg.player_y = m_player[0].y;
        m_send_msg.player1_score = m_player[0].score;
        m_send_msg.player2_score = m_player[1].score;

        // Send game information
        m_tcp->Send(m_send_msg);

        // Receive player 2 information
        Message recv_msg;
        if (m_tcp->Receive(recv_msg))
        {
            m_player[1].y = recv_msg.player_y;

            if (m_send_msg.command < recv_msg.command)
            {
                HandleCommand(recv_msg.command);
            }
        }
    }
    else if (m_type == GameType::Client)
    {
        // Build message
        m_send_msg.player_y = m_player[1].y;

        // Receive player 1 information
        Message recv_msg;
        if (m_tcp->Receive(recv_msg))
        {
            m_ball.x = recv_msg.ball_x;
            m_ball.y = recv_msg.ball_y;
            m_player[0].y = recv_msg.player_y;
            m_player[0].score = recv_msg.player1_score;
            m_player[1].score = recv_msg.player2_score;

            if (m_send_msg.command < recv_msg.command)
            {
                HandleCommand(recv_msg.command);
            }
        }

        // Send game information
        m_tcp->Send(m_send_msg);
    }

    m_send_msg.command = MessageControl::None;  // Reset message

    if (m_restart_on_update)
    {
        Restart();
    }
}

string DigitToString(int digit_)
{
    return string(1, char(digit_) + '0');
}

void Pong::Draw(SurfaceWindow *window_)
{
    window_->Clean();   // Start

    Surface *temp_surface = nullptr;
    switch (m_state)
    {
        case GameState::ChooseType:
            window_->Apply(&s_surface.at("type_key"), 2, 2);
            window_->Apply(&s_surface.at("type_local"), 6, 9);
            window_->Apply(&s_surface.at("type_server"), 6, 18);
            window_->Apply(&s_surface.at("type_client"), 6, 27);
            break;

        case GameState::Connect:

            window_->Apply(&s_surface.at("conn"), 2, 2);

            temp_surface = (m_type == GameType::Server)
                ? &s_surface.at("conn_client")
                : &s_surface.at("conn_server");

            window_->Apply(temp_surface, 2, 9);

            break;

        case GameState::Run:
        case GameState::Pause:
            // Players score
            for (size_t i = 0; i < 2; ++i)
            {
                window_->Apply(
                    &s_surface.at(DigitToString(m_player[i].score % 10)),
                    s_score_x[i].second, 1);
                window_->Apply(
                    &s_surface.at(DigitToString(m_player[i].score / 10)),
                    s_score_x[i].first, 1);
            }

            // Vertical bar
            window_->Apply(&s_surface.at("vertical"), CONFIG_WIDTH / 2, 0);

            // Player 1
            window_->Apply(&s_surface.at("player"), 0, (int)m_player[1].y);

            // Player 2
            window_->Apply(&s_surface.at("player"), CONFIG_WIDTH - 1, (int)m_player[0].y);

            // Ball
            window_->Apply(&s_surface.at("ball"), (int)m_ball.x, (int)m_ball.y);
            break;

        case GameState::End:
            window_->Apply(&s_surface.at("win"), 2, 2);

            temp_surface = (m_player[0].score > m_player[1].score)
                ? &s_surface.at("one")
                : &s_surface.at("two");

            window_->Apply(temp_surface, 2, 8);
            break;

        case GameState::Close:
            break;
    }

    window_->Draw();    // End
}

void Pong::Deinitialize()
{
    delete m_local_kbd; m_local_kbd = nullptr;
    delete m_tcp; m_tcp = nullptr;
}

bool Pong::IsRunning()
{
    return (GameState::Close != m_state);
}

void Pong::RespawnBall()
{
    m_ball.acceleration = CONFIG.ball.acceleration;
    m_ball.direction_x = ((rand() % 2) ? 1 : -1) / sqrt(2);
    m_ball.direction_y = ((rand() % 2) ? 1 : -1) / sqrt(2);
    m_ball.velocity = CONFIG.ball.velocity;
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
    m_player[0] = { float(CONFIG_HEIGHT - CONFIG.player.length) / 2,
                    0, CONFIG.player.first.up,  CONFIG.player.first.down,
                    m_local_kbd };
    m_player[1] = { float(CONFIG_HEIGHT - CONFIG.player.length) / 2,
                    0, CONFIG.player.second.up, CONFIG.player.second.down,
                    m_local_kbd };

    switch (m_type)
    {
        case GameType::Local:
            break;
        
        case GameType::Client:
            m_player[0].device = nullptr;   // Disable input
            m_tcp = new TcpClient<Message>(CONFIG.ip, CONFIG.port);
            break;

        case GameType::Server:
            m_player[1].device = nullptr;   // Disable input
            m_tcp = new TcpServer<Message>(CONFIG.port);
            break;
    }
}

void Pong::Restart()
{
    m_state = GameState::Run;
    m_restart_on_update = false;
    InitializePlayers();
    RespawnBall();
}

void Pong::HandleCommand(MessageControl command_)
{
    switch (command_)
    {
        case MessageControl::None:
            // Do nothing
            break;

        case MessageControl::ToPause:
            m_state = GameState::Pause;
            break;

        case MessageControl::ToContinue:
            m_state = GameState::Run;
            break;

        case MessageControl::ToEnd:
            m_state = GameState::End;
            break;

        case MessageControl::ToRestart:
            m_restart_on_update = true;
            break;

        case MessageControl::ToQuit:
            m_state = GameState::Close;
            break;
    }
}

/* -------------------------------------------------------------------------- *
 *  BMP Loader
 * -------------------------------------------------------------------------- */
#include <iostream>

static Surface LoadRaw(const string &path_, size_t width_, size_t height_)
{
    Surface ret(width_, height_);

    fstream file;
    file.open(path_, ios_base::in);
    if (file.fail()) THROW_ERROR;

    for (size_t y = 0; y < height_; ++y)
    {
        for (size_t x = 0; x < width_; ++x)
        {
            char buffer[5]= { 0 };
            file.get(buffer, 5);
            ret[y][x] = Pixel(buffer[0], buffer[1], buffer[2]);
        }
    }

    file.close();

    return ret;
}

/* -------------------------------------------------------------------------- *
 *  Pong Static Variables
 * -------------------------------------------------------------------------- */

unordered_map<string, Surface> Pong::s_surface{
    // Base
    { "player", Surface(1, CONFIG.player.length, W) },
    { "ball", Surface(1, 1, W) },
    { "vertical", Surface(1, CONFIG_HEIGHT, G) },

    // Score
    { "0", LoadRaw(CONFIG.resource_dir + "score_0.raw", 3, 5) },
    { "1", LoadRaw(CONFIG.resource_dir + "score_1.raw", 3, 5) },
    { "2", LoadRaw(CONFIG.resource_dir + "score_2.raw", 3, 5) },
    { "3", LoadRaw(CONFIG.resource_dir + "score_3.raw", 3, 5) },
    { "4", LoadRaw(CONFIG.resource_dir + "score_4.raw", 3, 5) },
    { "5", LoadRaw(CONFIG.resource_dir + "score_5.raw", 3, 5) },
    { "6", LoadRaw(CONFIG.resource_dir + "score_6.raw", 3, 5) },
    { "7", LoadRaw(CONFIG.resource_dir + "score_7.raw", 3, 5) },
    { "8", LoadRaw(CONFIG.resource_dir + "score_8.raw", 3, 5) },
    { "9", LoadRaw(CONFIG.resource_dir + "score_9.raw", 3, 5) },

    // Game type screen
    { "type_key", LoadRaw(CONFIG.resource_dir + "text_key.raw", 42, 5)},
    { "type_local", LoadRaw(CONFIG.resource_dir + "text_wait_local.raw", 23, 7)},
    { "type_server", LoadRaw(CONFIG.resource_dir + "text_wait_server.raw", 30, 7)},
    { "type_client", LoadRaw(CONFIG.resource_dir + "text_wait_client.raw", 26, 7)},

    // Connecting screen
    { "conn", LoadRaw(CONFIG.resource_dir + "text_conn_wait.raw", 38, 5)},
    { "conn_server", LoadRaw(CONFIG.resource_dir + "text_conn_server.raw", 29, 5)},
    { "conn_client", LoadRaw(CONFIG.resource_dir + "text_conn_client.raw", 29, 5)},

    // Win screen
    { "win", LoadRaw(CONFIG.resource_dir + "text_win.raw", 29, 17)},
    { "one", LoadRaw(CONFIG.resource_dir + "text_one.raw", 14, 5)},
    { "two", LoadRaw(CONFIG.resource_dir + "text_two.raw", 15, 5)},

};

array<pair<size_t, size_t>, 2> Pong::s_score_x{{
    { CONFIG_WIDTH / 2 + 2, CONFIG_WIDTH / 2 + 6 },
    { CONFIG_WIDTH / 2 - 8, CONFIG_WIDTH / 2 - 4 }
}};