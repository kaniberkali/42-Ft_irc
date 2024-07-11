
#ifndef Checker_HPP
# define Checker_HPP

#include <iostream>

class Checker
{   
    public:
        Checker(void);
        static int CheckerPort(std::string port);
        Checker(Checker const &Checker);
        ~Checker(void);
        Checker const &operator=(Checker const &Checker);
};

#endif
