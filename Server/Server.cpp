#include "Server.hpp"
#include "../Logger/Logger.hpp"
#include "../Utils/Utils.hpp"
#include <csignal>
#include "../Exception/ClientException.hpp"
#include "../Exception/ServerException.hpp"
#include "../Parser/Parser.hpp"
#include "../Command/Command.hpp"
#include "../Message/Message.hpp"
#include <cstring>
#include <vector>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>

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
        Logger::Warning("Closing client socket " + Utils::toString(_clients[i]->getFd().fd));
        Logger::Warning("Removing client " + Utils::toString(_clients[i]->getFd().fd));
        removeClient(_clients[i]->getFd().fd);
    }
    for (size_t i = 0; i < _channels.size(); i++)
    {
        Logger::Warning("Removing channel " + _channels[i]->getName());
        removeChannel(_channels[i]->getName());
    }
    Logger::Warning("Closing server socket " + Utils::toString(_socketFd.fd));
    ::close(_socketFd.fd);
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
    this->_name = DEFAULT_NAME;
    this->_version = DEFAULT_VERSION;
    this->_createdDate = Utils::date("D, d M Y H:i:s z");
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
    struct pollfd socketPoll;
    socketPoll.events = POLLIN;

    Logger::Info("Initializing server socket");
    socketPoll.fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketPoll.fd < 0)
        throw ServerException::SocketException();
    Logger::Trace("Ipv4, TCP protocol socket initialized");

    _serverAddr.sin_family = AF_INET;
    _serverAddr.sin_addr.s_addr = INADDR_ANY;
    _serverAddr.sin_port = htons(_port);

    int enabled = 1;
    Logger::Trace("Setting server address");
    if (setsockopt(socketPoll.fd, SOL_SOCKET, SO_REUSEADDR, &enabled, sizeof(int)) < 0)
        throw ServerException::SetSockOptException();
    Logger::Trace("Server address set");
    if (fcntl(socketPoll.fd, F_SETFL, O_NONBLOCK) < 0)
        throw ServerException::FcntlException();
    Logger::Trace("Server address initialized");
    if (bind(socketPoll.fd, (struct sockaddr *) &_serverAddr, sizeof(_serverAddr)) < 0)
        throw ServerException::BindException();
    Logger::Trace("Server socket binded to port " + Utils::toString(_port) + " successfully");
    if (::listen(socketPoll.fd, _maxClients) < 0)
        throw ServerException::ListenException();
    Logger::Info("Server socket initialized on port " + Utils::toString(_port) +
                 " with max clients " + Utils::toString(_maxClients) + " successfully");
    Logger::Info("Listening to socket " + Utils::toString(socketPoll.fd));
    _socketFd = socketPoll;
    listen();
}

void Server::listen(int fd, std::string hostName)
{
    struct pollfd socketPoll;
    socketPoll.fd = fd;
    socketPoll.events = POLLIN;
    Logger::Info("New client connecting");
    Client *client = new Client(socketPoll, hostName);
    _clients.push_back(client);
    Logger::Info("New client connected");

}

void Server::close(int fd)
{
    Logger::Info("Closing client socket " + Utils::toString(fd));
    for (size_t i = 0; i < _clients.size(); i++)
    {
        if (_clients[i]->getFd().fd == fd)
        {
            delete _clients[i];
            _clients.erase(_clients.begin() + i);
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
        if (_clients[i]->getFd().fd == fd)
        {
            delete _clients[i];
            _clients.erase(_clients.begin() + i);
            break;
        }
    }
    for (size_t i = 0; i < _channels.size(); i++)
        _channels[i]->removeClient(_channels[i]->getClient(fd));
    close(fd);
    Logger::Info("Client " + Utils::toString(fd) + " removed");
}

void Server::removeChannel(std::string name)
{
    for (size_t i = 0; i < _channels.size(); i++)
    {
        if (_channels[i]->getName() == name)
        {
            delete _channels[i];
            _channels.erase(_channels.begin() + i);
            break;
        }
    }
}

std::string Server::read(int fd)
{
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    Logger::Trace("Reading from client socket " + Utils::toString(fd));
    int readSize = recv(fd, buffer, BUFFER_SIZE, 0);
    if (readSize == 0)
    {
        Logger::Info("User client buffer not readed correctly beacuse client is disconnected.");
        removeClient(fd);
    }
    else if (readSize < 0)
        throw ServerException::ReadException();
    Logger::Trace("Read " + Utils::toString(readSize) + " bytes from client socket " + Utils::toString(fd));
    std::string message = std::string(buffer, readSize);
    if (message.find("\r\n") != std::string::npos)
    {
        std::vector<std::string> messages = Utils::split(message, "\r\n");
        for (size_t i = 0; i < messages.size(); i++)
            Logger::Trace("Read: " + Utils::trim(messages[i]));
    }
    else
        Logger::Trace("Read: " + Utils::trim(message));
    return message;
}

void Server::listen(void)
{
    int lastClientSize = 0;
    Logger::Info("Listening to clients");
    std::vector<pollfd> pollFds;
    pollFds.push_back(_socketFd);

    while (!_terminate)
    {
        for (size_t i = lastClientSize; i < _clients.size(); i++)
            pollFds.push_back(_clients[i]->getFd());
        lastClientSize = _clients.size();

        int pollResult = poll(pollFds.data(), pollFds.size(), TIMEOUT);
        if (pollResult < 0)
        {
            Logger::Error("Polling failed");
            quit();
            break;
        }

        if (pollResult == 0)
        {
            Logger::Trace("Server waiting for requests");
            continue;
        }

        if (pollFds[0].revents == POLLIN)
        {
            Logger::Trace("New client connecting");
            struct sockaddr_in clientAddr;
            socklen_t clientAddrLen = sizeof(clientAddr);
            int fd = accept(_socketFd.fd, (struct sockaddr *) &clientAddr, &clientAddrLen);
            if (fd < 0)
                throw ServerException::AcceptException();
            Logger::Info("New client connected");
            char hostName[NI_MAXHOST];
            getnameinfo((struct sockaddr*)&clientAddr, clientAddrLen, hostName, NI_MAXHOST, NULL, 0, 0);
            listen(fd, hostName);
        }

        for (size_t i = 1; i < pollFds.size(); i++)
        {
            if (pollFds[i].revents & POLLIN)
            {
                try
                {
                    std::string message = read(pollFds[i].fd);
                    Command::Execute(*this, message, pollFds[i].fd);
                } catch (ServerException::ReadException &e) {}
            }
        }
    }
}

Client* Server::getClient(int fd)
{
    for (size_t i = 0; i < _clients.size(); i++)
    {
        if (_clients[i]->getFd().fd == fd)
            return _clients[i];
    }
    return NULL;
}

Client* Server::getClient(std::string nickName)
{
    for (size_t i = 0; i < _clients.size(); i++)
    {
        if (_clients[i]->getNickName() == nickName)
            return _clients[i];
    }
    return NULL;
}

Client* Server::getClientByNickName(std::string nickname)
{
    for (size_t i = 0; i < _clients.size(); i++)
    {
        if (_clients[i]->getNickName() == nickname)
            return _clients[i];
    }
    return NULL;
}

void Server::addChannel(Channel *channel)
{
    _channels.push_back(channel);
}

Channel* Server::getChannel(std::string name)
{
    for (size_t i = 0; i < _channels.size(); i++)
    {
        if (_channels[i]->getName() == name)
            return _channels[i];
    }
    return NULL;
}

std::string Server::getName()
{
    return _name;
}

std::string Server::getVersion()
{
    return _version;
}

Channel *Server::getChannel(int index)
{
    return _channels[index];
}

std::vector<Channel *> Server::getChannels()
{
    return _channels;
}

std::vector<Client *> Server::getClients()
{
    return _clients;
}

std::string Server::getCreateDate()
{
    return _createdDate;
}

serverInfo Server::getInfo()
{
    serverInfo info;
    info.name = _name;
    info.password = _password;
    info.version = _version;
    info.createDate = _createdDate;
    return info;
}