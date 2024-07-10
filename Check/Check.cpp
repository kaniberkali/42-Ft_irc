#include "Check.hpp"


Check::Check(void) : _port(0), _pass("") {}

Check::Check(int port, std::string pass) : _port(port), _pass(pass) {
    this->CheckPort(port);
    this->CheckPass(pass); // ... ı am here 
}

