#ifndef __ENDPOINT_HPP__
#define __ENDPOINT_HPP__
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- *
 *  Libraries
 * -------------------------------------------------------------------------- */

#include <sys/types.h>
    // struct sockaddr

#include <string>
    using std::string;

/* -------------------------------------------------------------------------- *
 *  Libraries
 * -------------------------------------------------------------------------- */

enum ConnectionState
{
    Connecting,
    Connected,
    Disconnected
};

/* -------------------------------------------------------------------------- *
 *  Interface
 * -------------------------------------------------------------------------- */

template<typename SendMsg, typename ReceiveMsg>
class EndPoint
{
public:
    explicit EndPoint() = default;
    virtual ~EndPoint() = 0;

    // Uncopyable
    EndPoint(const EndPoint&) = delete;
    EndPoint& operator=(const EndPoint&) = delete;

    // Connect to another endpoint
    virtual void Connect() = 0;

    // Disconnect current endpoint
    virtual void Disconnect() = 0;

    // Send message to other endpoint
    virtual void Send(const SendMsg &msg_) = 0;

    // Receive message from other endpoint
    virtual bool Receive(ReceiveMsg &msg_) = 0;

    // Return the current state of the endpoint
    virtual ConnectionState GetState() const = 0;
};

template<typename SendMsg, typename ReceiveMsg>
inline EndPoint<SendMsg, ReceiveMsg>::~EndPoint() { }

/* -------------------------------------------------------------------------- */
#endif /* __ENDPOINT_HPP__ */