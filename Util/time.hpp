#pragma once

#include <ctime>
#include <iomanip>

namespace Util
{

std::string getTime(const std::string &format)
{
    auto t  = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::stringstream ss;
    ss << std::put_time(&tm, format.c_str());
    return ss.str();
}

} // namespace Util
