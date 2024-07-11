
#ifndef CHECK_HPP
# define CHECK_HPP

#include <iostream>

class Check
{   
    public:
        Check(void);
        static int CheckPort(std::string port);
        Check(Check const &check);
        ~Check(void);
        Check const &operator=(Check const &check);
};

#endif
