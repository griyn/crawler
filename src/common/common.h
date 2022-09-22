#pragma once

#include <time.h>
#include <iomanip>
#include <sstream>

namespace garden {
namespace common {

std::string get_time_of_day_d() {
    std::stringstream ss;
    time_t now = time(nullptr);
    ss << std::put_time(localtime(&now), "%Y%m%d");
    return ss.str();
}

} // commmon
} // garden