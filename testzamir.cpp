

#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>



#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

#define BOLD    "\033[1m"
#define UNDERLINE "\033[4m"



static inline void log(const std::string& color, const std::string& message) 
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    std::cout << "\033[0;34m[" 
              << std::put_time(std::localtime(&now_c), "%d-%m-%Y %H:%M:%S") 
              << "]\033[0m " 
              << color << message << "\033[0m" << std::endl;
}

int main() {
    log(RED, "This is a test log message.");
    log(BLUE,"This is a test log message.");
    return 0;
}
