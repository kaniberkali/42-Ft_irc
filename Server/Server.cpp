#include "Server.hpp"
#include "../Logger/Logger.hpp"
#include "../Utils/Utils.hpp"
#include <csignal>
#include "../Exception/ServerException.hpp"
#include <stdlib.h>
#include <cstring>
Server::Server()
{
    Logger::Info("Server started");
    Server(DEFAULT_PORT, DEFAULT_PASSWORD);
}

Server::~Server()
{
    Logger::Info("Server stopped");
}

void signalHandler(int signal)
{
    Logger::Info("Signal " + Utils::toString(signal) + " received");
    //this->_terminate = true;
}

Server::Server(int port, std::string password) : _port(port), _password(password)
{
    this->_maxClients = MAX_CLIENTS;
    this->_terminate = false;
    Logger::Info("Server starting on port " + Utils::toString(port) + " with password " + password);
    signal(SIGINT, &signalHandler);
    Logger::Trace("Signal INT handled");
    signal(SIGQUIT, &signalHandler);
    Logger::Trace("Signal QUIT handled");
    initSocket();
    Logger::Info("Server started on port " + Utils::toString(port) + " with password " + password);
}

Server::Server(Server const &server)
{
    *this = server;
}

Server const &Server::operator=(Server const &server)
{
    _port = server._port;
    _password = server._password;
    _maxClients = server._maxClients;
    return *this;
}

int Server::getPort(void) const
{
    return _port;
}

std::string Server::getPassword(void) const
{
    return _password;
}

void Server::setSettings(int port, std::string password, int maxClients)
{
    _port = port;
    _password = password;
    _maxClients = maxClients;
}

void Server::quit(void)
{
    _terminate = true;
}

void Server::initSocket(void)
{
    Logger::Info("Initializing server socket");
    _socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_socketFd < 0)
        throw ServerException::SocketException();
    Logger::Trace("Ipv4, TCP protocol socket initialized");

    _serverAddr.sin_family = AF_INET;
    _serverAddr.sin_addr.s_addr = INADDR_ANY;
    _serverAddr.sin_port = htons(_port);

    Logger::Trace("Server address initialized");
    if (bind(_socketFd, (struct sockaddr *)&_serverAddr, sizeof(_serverAddr)) < 0)
        throw ServerException::BindException();
    Logger::Trace("Server socket binded to port " + Utils::toString(_port) + " successfully");
    if (listen(_socketFd, _maxClients) < 0)
        throw ServerException::ListenException();
    Logger::Info("Server socket initialized on port " + Utils::toString(_port) + " with max clients " + Utils::toString(_maxClients) + " successfully");

    Logger::Debug("Test for one client");
    int client = accept(_socketFd, NULL, NULL);
    if (client < 0)
        throw ServerException::AcceptException();
    Logger::Debug("Test for one client passed");
    char buffer[BUFFER_SIZE];
    while (true)
    {
        memset(buffer, 0, BUFFER_SIZE);
        int read = recv(client, buffer, BUFFER_SIZE, 0);
        if (read < 0)
            throw ServerException::ReceiveException();
        if (read == 0)
            break;
        Logger::Info("Client message: " + std::string(buffer));
        send(client, buffer, read, 0);
    }
    close(client);
}