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

template<typename SendMsg, typename ReceiveMsg>
class Tcp : public EndPoint<SendMsg, ReceiveMsg>
{
public:
    explicit Tcp(const string &addr_, int port_);
    virtual ~Tcp() = 0;

    void Send(const SendMsg &msg_) override;
    bool Receive(ReceiveMsg &msg_) override;

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

template<typename SendMsg, typename ReceiveMsg>
class TcpServer : public Tcp<SendMsg, ReceiveMsg>
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

template<typename SendMsg, typename ReceiveMsg>
class TcpClient : public Tcp<SendMsg, ReceiveMsg>
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

template<typename SendMsg, typename ReceiveMsg>
Tcp<SendMsg, ReceiveMsg>::Tcp(const string &addr_, int port_) :
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

template<typename SendMsg, typename ReceiveMsg>
Tcp<SendMsg, ReceiveMsg>::~Tcp()
{
    close(m_socket);
}

template<typename SendMsg, typename ReceiveMsg>
void Tcp<SendMsg, ReceiveMsg>::Send(const SendMsg &msg_)
{
    write(m_socket, &msg_, sizeof(SendMsg));
}

template<typename SendMsg, typename ReceiveMsg>
bool Tcp<SendMsg, ReceiveMsg>::Receive(ReceiveMsg &msg_)
{
    int msg_size = read(m_socket, &msg_, sizeof(ReceiveMsg));

    if (0 == msg_size)
    {
        this->Disconnect();
    }
    
    return (msg_size > 0);
}

template<typename SendMsg, typename ReceiveMsg>
ConnectionState Tcp<SendMsg, ReceiveMsg>::GetState() const
{
    return m_state;    
}

template<typename SendMsg, typename ReceiveMsg>
sockaddr *Tcp<SendMsg, ReceiveMsg>::GetAddress()
{
    return reinterpret_cast<sockaddr*>(&m_addr);
}

/* -------------------------------------------------------------------------- *
 *  TCP server implementation
 * -------------------------------------------------------------------------- */

template<typename SendMsg, typename ReceiveMsg>
TcpServer<SendMsg, ReceiveMsg>::TcpServer(int port_) :
    Tcp<SendMsg, ReceiveMsg>("127.0.0.1", port_),
    m_server_socket(this->m_socket)
{
    this->m_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((0 > bind(this->m_socket, this->GetAddress(), sizeof(sockaddr_in))) ||
        (0 > listen(this->m_socket, 1)))
    {
        THROW_ERROR;
    }
}

template<typename SendMsg, typename ReceiveMsg>
TcpServer<SendMsg, ReceiveMsg>::~TcpServer()
{
    this->Disconnect();
}

template<typename SendMsg, typename ReceiveMsg>
void TcpServer<SendMsg, ReceiveMsg>::Connect()
{
    socklen_t len = sizeof(sockaddr_in);
    int accepted = accept(this->m_socket, this->GetAddress(), &len);
    if (0 > accepted) return;

    this->m_socket = accepted;
    this->m_state = ConnectionState::Connected;
}

template<typename SendMsg, typename ReceiveMsg>
void TcpServer<SendMsg, ReceiveMsg>::Disconnect()
{
    close(m_server_socket);
    this->m_state = ConnectionState::Disconnected;
}

/* -------------------------------------------------------------------------- *
 *  TCP client implementation
 * -------------------------------------------------------------------------- */

template<typename SendMsg, typename ReceiveMsg>
TcpClient<SendMsg, ReceiveMsg>::TcpClient(const string &addr_, int port_) :
    Tcp<SendMsg, ReceiveMsg>(addr_, port_)
{
    // Do nothing
}

template<typename SendMsg, typename ReceiveMsg>
TcpClient<SendMsg, ReceiveMsg>::~TcpClient()
{
    this->Disconnect();
}

template<typename SendMsg, typename ReceiveMsg>
void TcpClient<SendMsg, ReceiveMsg>::Connect()
{
    int connect_result = connect(this->m_socket, this->GetAddress(), sizeof(sockaddr_in));
    if (0 > connect_result) return;

    this->m_state = ConnectionState::Connected;
}

template<typename SendMsg, typename ReceiveMsg>
void TcpClient<SendMsg, ReceiveMsg>::Disconnect()
{
    this->m_state = ConnectionState::Disconnected;
}

/* -------------------------------------------------------------------------- */
#endif /* __TCP_HPP__ */