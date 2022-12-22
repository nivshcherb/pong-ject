/* -------------------------------------------------------------------------- *
 *  Libraries
 * -------------------------------------------------------------------------- */

#include <thread>
    using std::thread;

#include <iostream>

#include "tcp.hpp"

using namespace std;
using namespace chrono;

/* -------------------------------------------------------------------------- *
 *  Misc
 * -------------------------------------------------------------------------- */

constexpr static int PORT = 5000;
const static string ADDRESS("127.0.0.1");

/* -------------------------------------------------------------------------- *
 *  Main
 * -------------------------------------------------------------------------- */


void RunServer()
{
    TcpServer<int> server(PORT);

    cout << "server setup" << endl;

    while (ConnectionState::Connecting == server.GetState())
    {
        this_thread::sleep_for(milliseconds(500));
        server.Connect();
        cout << "server wait" << endl;
    }

    cout << "server connected" << endl;

    int message = 1;
    while (ConnectionState::Connected == server.GetState())
    {
        if (message > 5) break;

        server.Send(message);
        ++message;
    }

    cout << "server disconnected" << endl;
}

void RunClient()
{
    TcpClient<int> client(ADDRESS, PORT);

    this_thread::sleep_for(milliseconds(250));

    cout << "client setup" << endl;

    while (ConnectionState::Connecting == client.GetState())
    {
        this_thread::sleep_for(milliseconds(500));
        client.Connect();
        cout << "client wait" << endl;
    }

    cout << "client connected" << endl;

    int message = 0;
    while (ConnectionState::Connected == client.GetState())
    {
        if (client.Receive(message))
        {
            cout << message << endl;
        }
    }

    cout << "client disconnected" << endl;
}

int main()
{
    thread server(RunServer);
    thread client(RunClient);

    server.join();
    client.join();

    cout << flush;

    return 0;
}