#include <iostream>
#include "includes/Server.hpp"

int main(int argc, char const *argv[])
{
    if (argc > 3)
        throw (std::runtime_error("Guide: ./ircserv <port> <password>"));
    Server server(argv[1], argv[2]);
    try
    {
        server.start();
        return (0);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return (1);
    }
}
