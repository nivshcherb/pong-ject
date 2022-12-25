# Tcp

<img src="https://media.tenor.com/dKj-01-GggoAAAAC/network-connecting.gif" alt="tcp"/>

To allow online play, we are going to add some generic connection class that will be used to synchronize the information of the game between a server player and client player. It is also possible to create a server and have each player connect to it.

## Endpoint

We require some simple methods from out connection. We need to be able to connnect, disconnect, and exchange messages between the server and client. In addition, knowing the connection status will help us decide how to handle the current situation.

Since these method are required from both the server and client, we will first create *Endpoint* interface.

Method      |   Description
:--         |   :--
Connect     |   Connect to another endpoint
Disconnect  |   Disconnect from the other endpoint
Send        |   Send some message to the connected endpoint
Receive     |   Receive a message from the connected endpoint
GetState    |   Return the status of the current connection

## Tcp

Next we would like to implement *Endpoint* with tcp server and tcp client.
Since they share most of their functionality and only differ in the methods *Connect()* and *Disconnect()*, we first create an abstract *TCP* class.

*TCP* will handle a socket in it's *Ctor()* and *Dtor()*, and use it in *Send()* and *Recieve*. Think about the properties you want to give the socket. Should it block? Why yes or no.

Finaly, derive 2 new classes from *TCP*, *TcpServer* and *TcpClient* and implement the methods *Connect()* and *Disconnect()*. It's enough for the server to only accept a single client if you decide on having one server one client.

## Online Pong

Add states to your Pong to allow choosing local / online play and waiting for connections.
For online play, the server will handle the logic and send it to the client. In return, the client will send it's player location or the current keys pressed.

## Relevant Files

- [interface/endpoint.hpp](../Pong/interface/endpoint.hpp)
- [include/tcp.hpp](../Pong/include/tcp.hpp)


