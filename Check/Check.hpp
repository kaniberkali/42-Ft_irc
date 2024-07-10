
#ifndef CHECK_HPP
# define CHECK_HPP

#include <iostream>

class Check
{   
    private:
        int _port;
        std::string _pass;

    public:
        Check(void);
        Check(int port , std::string pass ); // belki 
        //bool CheckPort(int port);
        //bool CheckPass(std::string pass);
        Check(Check const &check);
        ~Check(void);
        Check const &operator=(Check const &check);
};

#endif
