#pragma once

#include <iostream>
#include <string>
#include <ostream>
#include <sstream>
#include <time.h>
#include <iomanip>

class LogStream {
public:
    ~LogStream() {
        std::cerr << _oss.str() << std::endl;
    }

    template<typename T>
    LogStream& operator<<(T const& t) {
        _oss << t;
        return *this;
    }

private:
    std::stringstream _oss;
};

#define LOG(level) \
    time_t now = time(nullptr); \
    LogStream() << #level << " " << std::put_time(localtime(&now), "%Y-%m-%d %H:%M:%S") \
        << " * " << __FILE__ << ":" << __LINE__ << "] "

/* example:
int main() {
    LOG(NOTICE) << 1 << 2 << 3;
    return 0;
}

NOTICE 2022-09-05 18:53:06 * test.cpp:30] 123
*/