#include "game_string_utils.h"

std::vector<std::string> game_string_utils::split(std::string str, std::string pattern)
{
    std::vector<std::string> result;
    str += pattern;
    auto size = str.size();
    for (size_t i = 0; i < size; i++)
    {
        auto pos = str.find(pattern, i);
        if (pos < size)
        {
            auto s = str.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}
