#ifndef __TCP_HPP__
#define __TCP_HPP__
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- *
 *  Libraries
 * -------------------------------------------------------------------------- */

#include <queue>
    using std::queue;

#include <thread>
    using std::thread;

#include <system_error>
    using std::system_error;
    using std::generic_category;

#include <fcntl.h>
    // fcntl

#include <unistd.h>
    // read
    // write

#include <arpa/inet.h>
    // inet_addr

#include <netinet/in.h>
    // struct sockaddr_in

#include <sys/socket.h> 
    // bind
    // recvfrom
    // sendto

#include <cerrno>
    // errno  

#include "endpoint.hpp"

/* -------------------------------------------------------------------------- *
 *  Misc
 * -------------------------------------------------------------------------- */

#define THROW_ERROR throw(system_error(errno, generic_category()))

/* -------------------------------------------------------------------------- *
 *  Abstract tcp connection
 * -------------------------------------------------------------------------- */

template<typename Msg>
class Tcp : public EndPoint<Msg>
{
public:
    explicit Tcp(const string &addr_, int port_);
    virtual ~Tcp() = 0;

    void Send(const Msg &msg_) override;
    bool Receive(Msg &msg_) override;

    ConnectionState GetState() const override;

protected:
    sockaddr *GetAddress();

    // Members
    ConnectionState m_state;
    int m_socket;
    sockaddr_in m_addr;
};

/* -------------------------------------------------------------------------- *
 *  TCP server
 * -------------------------------------------------------------------------- */

template<typename Msg>
class TcpServer : public Tcp<Msg>
{
public:
    explicit TcpServer(int port_);
    ~TcpServer();

    void Connect() override;
    void Disconnect() override;

private:
    int m_server_socket;
};

/* -------------------------------------------------------------------------- *
 *  TCP client
 * -------------------------------------------------------------------------- */

template<typename Msg>
class TcpClient : public Tcp<Msg>
{
public:
    explicit TcpClient(const string &addr_, int port_);
    ~TcpClient();

    void Connect() override;
    void Disconnect() override;
};

/* -------------------------------------------------------------------------- *
 *  Abstract tcp connection implementation
 * -------------------------------------------------------------------------- */

template<typename Msg>
Tcp<Msg>::Tcp(const string &addr_, int port_) :
    m_state(ConnectionState::Connecting),
    m_socket(0),
    m_addr{ 0 }
{
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (0 > m_socket)
    {
        THROW_ERROR;
    }

    int sock_flags = fcntl(m_socket, F_GETFL);
    if (0 > fcntl(m_socket, F_SETFL, sock_flags | O_NONBLOCK))
    {
        THROW_ERROR;
    }

    m_addr.sin_family = AF_INET;
    m_addr.sin_port = htons(port_);
    m_addr.sin_addr.s_addr = inet_addr(addr_.c_str());
}

template<typename Msg>
Tcp<Msg>::~Tcp()
{
    close(m_socket);
}

template<typename Msg>
void Tcp<Msg>::Send(const Msg &msg_)
{
    write(m_socket, &msg_, sizeof(Msg));
}

template<typename Msg>
bool Tcp<Msg>::Receive(Msg &msg_)
{
    int msg_size = read(m_socket, &msg_, sizeof(Msg));

    if (0 == msg_size)
    {
        this->Disconnect();
    }
    
    return (msg_size > 0);
}

template<typename Msg>
ConnectionState Tcp<Msg>::GetState() const
{
    return m_state;    
}

template<typename Msg>
sockaddr *Tcp<Msg>::GetAddress()
{
    return reinterpret_cast<sockaddr*>(&m_addr);
}

/* -------------------------------------------------------------------------- *
 *  TCP server implementation
 * -------------------------------------------------------------------------- */

template<typename Msg>
TcpServer<Msg>::TcpServer(int port_) :
    Tcp<Msg>("127.0.0.1", port_),
    m_server_socket(this->m_socket)
{
    this->m_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((0 > bind(this->m_socket, this->GetAddress(), sizeof(sockaddr_in))) ||
        (0 > listen(this->m_socket, 1)))
    {
        THROW_ERROR;
    }
}

template<typename Msg>
TcpServer<Msg>::~TcpServer()
{
    this->Disconnect();
}

template<typename Msg>
void TcpServer<Msg>::Connect()
{
    socklen_t len = sizeof(sockaddr_in);
    int accepted = accept(this->m_socket, this->GetAddress(), &len);
    if (0 > accepted) return;

    this->m_socket = accepted;
    this->m_state = ConnectionState::Connected;
}

template<typename Msg>
void TcpServer<Msg>::Disconnect()
{
    close(m_server_socket);
    this->m_state = ConnectionState::Disconnected;
}

/* -------------------------------------------------------------------------- *
 *  TCP client implementation
 * -------------------------------------------------------------------------- */

template<typename Msg>
TcpClient<Msg>::TcpClient(const string &addr_, int port_) :
    Tcp<Msg>(addr_, port_)
{
    // Do nothing
}

template<typename Msg>
TcpClient<Msg>::~TcpClient()
{
    this->Disconnect();
}

template<typename Msg>
void TcpClient<Msg>::Connect()
{
    int connect_result = connect(this->m_socket, this->GetAddress(), sizeof(sockaddr_in));
    if (0 > connect_result) return;

    this->m_state = ConnectionState::Connected;
}

template<typename Msg>
void TcpClient<Msg>::Disconnect()
{
    this->m_state = ConnectionState::Disconnected;
}

/* -------------------------------------------------------------------------- */
#endif /* __TCP_HPP__ */