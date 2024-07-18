#include "Server.hpp"
#include "../Logger/Logger.hpp"
#include "../Utils/Utils.hpp"
#include <csignal>
#include "../Exception/ServerException.hpp"
#include "../Parser/Parser.hpp"
#include "../Command/Command.hpp"
#include <cstring>

bool Server::_terminate = false;

Server::Server()
{
    Logger::Info("Server started");
    Server(DEFAULT_PORT, DEFAULT_PASSWORD);
}

Server::~Server()
{
    for (size_t i = 0; i < _clients.size(); i++)
    {
        Logger::Warning("Closing client socket " + Utils::toString(_fds[i].fd));
        Logger::Warning("Removing client " + Utils::toString(_clients[i]->getFd()));
        removeClient(_clients[i]->getFd());
    }
    Logger::Warning("Closing server socket " + Utils::toString(_socketFd));
    ::close(_socketFd);
    Logger::Error("Server stopped");
}

void signalHandler(int signal)
{
    Logger::Info("Signal " + Utils::toString(signal) + " received");
    Server::quit();
}

Server::Server(int port, std::string password) : _port(port), _password(password)
{
    this->_maxClients = MAX_CLIENTS;
    this->_terminate = false;
    Logger::Info("Server starting on port " + Utils::toString(port) + " with password " + password);
    signal(SIGQUIT, &signalHandler);
    Logger::Trace("Signal QUIT handled");
    signal(SIGINT, &signalHandler);
    Logger::Trace("Signal INT handled");
    init();
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

void Server::quit(void)
{
    _terminate = true;
}

void Server::init(void)
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
    if (bind(_socketFd, (struct sockaddr *) &_serverAddr, sizeof(_serverAddr)) < 0)
        throw ServerException::BindException();
    Logger::Trace("Server socket binded to port " + Utils::toString(_port) + " successfully");
    if (::listen(_socketFd, _maxClients) < 0)
        throw ServerException::ListenException();
    Logger::Info("Server socket initialized on port " + Utils::toString(_port) +
                 " with max clients " + Utils::toString(_maxClients) + " successfully");
    listen(_socketFd);
    listen();
}

void Server::listen(int fd)
{
    Logger::Info("Listening to socket " + Utils::toString(fd));
    struct pollfd socketPoll;
    socketPoll.fd = fd;
    socketPoll.events = POLLIN;
    _fds.push_back(socketPoll);
}


void Server::newClient(int fd, clientInfo info)
{
    Logger::Info("New client connecting");
    Client *client = new Client(fd, info);
    _clients.push_back(client);
    Logger::Info("New client connected");
}

void Server::close(int fd)
{
    Logger::Info("Closing client socket " + Utils::toString(fd));
    for (size_t i = 0; i < _fds.size(); i++)
    {
        if (_fds[i].fd == fd)
        {
            _fds.erase(_fds.begin() + i);
            break;
        }
    }
    ::close(fd);
    Logger::Info("Client socket " + Utils::toString(fd) + " closed");
}

void Server::removeClient(int fd)
{
    Logger::Info("Removing client " + Utils::toString(fd));
    for (size_t i = 0; i < _clients.size(); i++)
    {
        if (_clients[i]->getFd() == fd)
        {
            delete _clients[i];
            _clients.erase(_clients.begin() + i);
            break;
        }
    }
    close(fd);
    Logger::Info("Client " + Utils::toString(fd) + " removed");
}

std::string Server::read(int fd)
{
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    Logger::Trace("Reading from client socket " + Utils::toString(fd));
    int readSize = recv(fd, buffer, BUFFER_SIZE, 0);
    if (readSize < 0)
    {
        Logger::Error("Failed to read from client socket " + Utils::toString(fd));
        throw ServerException::ReadException();
    }
    Logger::Trace("Read " + Utils::toString(readSize) + " bytes from client socket " + Utils::toString(fd));
    return std::string(buffer, readSize);
}

void Server::listen(void)
{
    Logger::Info("Listening to clients");
    while (!_terminate)
    {
        if (poll(&_fds[0], _fds.size(), 6000) < 0)
        {
            Logger::Error("Polling failed");
            quit();
            break;
        }
        if (_fds[0].revents == POLLIN)
        {
            Logger::Trace("New client connecting");
            struct sockaddr_in clientAddr;
            socklen_t clientAddrLen = sizeof(clientAddr);
            int fd = accept(_socketFd, (struct sockaddr *) &clientAddr, &clientAddrLen);
            if (fd < 0)
                throw ServerException::AcceptException();
            Logger::Info("New client connected");
            listen(fd);
        }
        Logger::Info("Reading from clients");
        for (size_t i = 1; i < _fds.size(); i++)
        {
            if (_fds[i].revents == POLLIN)
            {
                std::string message = read(_fds[i].fd);
                Logger::Info("Received message from client " + Utils::toString(_fds[i].fd) + ": " + message);
                Command::Perform(*this, message, _fds[i].fd);
            }
        }
    }
}
