#include "Server.hpp"
#include "../Logger/Logger.hpp"
#include "../Utils/Utils.hpp"
#include <csignal>
#include "../Exception/ServerException.hpp"
#include "../Parser/Parser.hpp"

bool Server::_terminate = false;

Server::Server()
{
    Logger::Info("Server started");
    Server(DEFAULT_PORT, DEFAULT_PASSWORD);
}

Server::~Server()
{
    for (size_t i = 0; i < _fds.size(); i++)
    {
        Logger::Warning("Closing client socket " + Utils::toString(_fds[i].fd));
        close(_fds[i].fd);
    }
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
    if (_socketFd != fd)
    {
        std::string message = read(fd);
        clientInfo info = Parser::connectionMessage(message);
        newClient(fd, info);
        Logger::Debug(info.nickName + " connected with real name " + info.realName + " and username " + info.userName);
        Logger::Info(info.realName + " connected with nickname " + info.nickName);
    }
}

void Server::newClient(int fd, clientInfo info)
{
    Client *client = new Client(fd, info);
    _clients.push_back(client);
    Logger::Info("New client connected");
}

void Server::read()
{
    Logger::Info("Reading from clients");
    for (size_t i = 1; i < _fds.size(); i++)
    {
        if (_fds[i].revents == POLLIN)
        {
            Logger::Trace("Reading from client socket " + Utils::toString(_fds[i].fd));
            std::string message = read(_fds[i].fd);
            Logger::Info("Message received from client socket " + Utils::toString(_fds[i].fd));
        }
    }
}

std::string Server::read(int fd)
{
    char buffer[BUFFER_SIZE];
    int readSize = recv(fd, buffer, BUFFER_SIZE, 0);
    if (readSize < 0)
        throw ServerException::ReadException();
    return std::string(buffer);
}

void Server::listen(void)
{
    Logger::Info("Listening to clients");
    while (!_terminate) {
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
        read();
    }
}
