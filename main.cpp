#include <iostream>
#include "includes/Server.hpp"

int main(int argc, char *argv[])
{
    if (argc != 3)
        throw (std::runtime_error("Guide: ./ircserv <port> <password>"));
    try
    {
        Server server(argv[1], argv[2]);
        server.start();
        return (0);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return (1);
    }
    return (0);
}
