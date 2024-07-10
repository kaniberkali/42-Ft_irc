
#include <iostream>

int main(int ac, char **av)
{
    if (ac == 3)
    {
        int portNum = av[1];
        std::string pass = av[2];
    }
    else {
        std::cerr << "yetersiz argüman sayisi" << std::endl;
    }


    return (0);
}